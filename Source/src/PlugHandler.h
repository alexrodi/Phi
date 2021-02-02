/*
  ==============================================================================

    PlugHandler.h
    Created: 1 Feb 2021 8:58:27pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

struct PlugID
{
    PlugID(std::pair<uint32, uint32> moduleplugID) :
    ownID( (uint64(moduleplugID.first) << 32) | uint64(moduleplugID.second) )
    {}

    PlugID(uint64 moduleplugID) : ownID(moduleplugID)
    {}
    
    PlugID(int64 moduleplugID) : ownID(moduleplugID)
    {}
    
    PlugID(String moduleplugIDString) : ownID(moduleplugIDString.toUTF8().getIntValue64())
    {}
    
    PlugID() : ownID(0)
    {}
    
    PlugID operator= (uint64 other) { ownID = other; return *this; }
    
    uint32 moduleID() const noexcept
    {
        return uint32(ownID >> 32);
    }
    
    uint32 plugID() const noexcept
    {
        return uint32(ownID & 0x00000FFFFF);
    }
    
    String toString() const noexcept
    {
        return String(ownID);
    }
    
    operator uint64() const noexcept {return ownID;}
    operator int64() const noexcept {return ownID;}
    
    bool operator== (const PlugID& other) const noexcept
    {
        return other.ownID == ownID;
    }
    
private:
    uint64 ownID;
};

/// This Class may exist in one of two modes
enum PlugMode { Inlet, Outlet };

/// An Event generated by a Plug
struct PlugEvent {
    virtual ~PlugEvent() {};
    
    template<class T>
    const T* as() const {
        return dynamic_cast<T*>(this);
    }
};

struct PlugMouseEvent : public PlugEvent {
    enum Type {
        MouseDown,
        MouseUp,
        MouseDrag
    };
    
    PlugMouseEvent(const PlugMode& plugMode, const PlugID& plugID, const MouseEvent& mouseEvent, const Type type):
    plugMode(plugMode),
    plugID(plugID),
    mouseEvent(mouseEvent),
    type(type)
    {}
    
    const PlugMode& plugMode;
    const PlugID& plugID;
    const MouseEvent& mouseEvent;
    const Type type;
};

struct PlugEndDragEvent : public PlugEvent {};

class PlugListener {
public:
    virtual ~PlugListener() = default;
    virtual void onPlugEvent(const PlugEvent& event) = 0;
};

class PlugEventEmitter {
public:
    // These methods are used by objects that want to subscribe and unsubscribe from changes that occur in this plug
    void addListener(PlugListener *l) {listeners.add(l);}
    void removeListener(PlugListener *l) {listeners.remove(l);}
    
    template<class T> void emitEvent(const T event) {
         // call the onPlugEvent method of every listener that has subscribed to be notified
         // pass in the value so that listeners can use it however they please
          listeners.call([event](PlugListener &l){ l.onPlugEvent(event); });
    }
    
private:
    ListenerList<PlugListener> listeners;
};

/// The PlugHandler implements patching mechanics between sources and destinations - dragging, clicking and keeping connected with the Shift key
class PlugHandler: public PlugListener {
public:
    bool isHoldingConnection() { return isConnecting; }
    void abortConnection() { isConnecting = false; onConnectionRelease(); }


    virtual void onConnectionStart (PlugMode plugMode, PlugID plugID) {};
    virtual void onConnectionDrag () {};
    virtual void onConnectionRelease () {};
    virtual void onConnectionEnd (std::pair<PlugID, PlugID> sourceDestination) {};

private:
    bool isConnecting;
    PlugMode originMode;
    PlugID sourcePlug, destinationPlug;
    
    void startConnection()
    {
        onConnectionStart(originMode, originMode == PlugMode::Inlet ? destinationPlug : sourcePlug);
    }
    
    void endConnection(const PlugMouseEvent& e)
    {
        // Complete connection pair
        if (originMode == PlugMode::Inlet) {
            sourcePlug = e.plugID;
        } else {
            destinationPlug = e.plugID;
        }
        
        bool keepConnection = e.mouseEvent.mods.isShiftDown();
        
        // Hold connection when shift is down
        isConnecting = keepConnection;
        if (!keepConnection) onConnectionRelease();
        
        // Only accept connection if the plugs are of opposite modes and from different modules
        if (originMode != e.plugMode
            &&
            sourcePlug.moduleID() != destinationPlug.moduleID())
        {
            onConnectionEnd({sourcePlug, destinationPlug});
        }
    }
    
    void onPlugEvent (const PlugEvent& event) override
    {
        if (auto e = event.as<const PlugMouseEvent>()) {
            
            if (e->type ==  PlugMouseEvent::Type::MouseDrag) {
                onConnectionDrag();
            } else if (e->type ==  PlugMouseEvent::Type::MouseDown) {
                if (isConnecting) {
                    endConnection(*e);
                } else {
                    if (e->plugMode == PlugMode::Inlet) {
                        destinationPlug = e->plugID;
                        sourcePlug = PlugID();
                    } else {
                        destinationPlug = PlugID();
                        sourcePlug = e->plugID;
                    }
                    originMode = e->plugMode;
                    isConnecting = true;
                    startConnection();
                }
            } else if (e->type ==  PlugMouseEvent::Type::MouseUp) {
                if (isConnecting && e->mouseEvent.mouseWasDraggedSinceMouseDown())
                    endConnection(*e);
            }
        } else if (auto e = event.as<const PlugEndDragEvent>()) {
            isConnecting = false;
            onConnectionRelease();
        }
    }
};
