/*
  ==============================================================================

    Plugs.h
    Created: 16 Apr 2020 11:08:52pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

///@cond
#include <JuceHeader.h>
///@endcond

//==============================================================================
namespace PlugOptions
{
    extern bool drawName;
}

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
    
    uint32 moduleID()
    {
        return uint32(ownID >> 32);
    }
    
    uint32 plugID()
    {
        return uint32(ownID & 0x00000FFFFF);
    }
    
    String toString()
    {
        return String(ownID);
    }
    
    operator uint64() {return ownID;}
    operator int64() {return ownID;}
    
    bool operator== (const PlugID& other) const noexcept
    {
        return other.ownID == ownID;
    }
    
private:
    uint64 ownID;
};

/// A base class for Inlet and Outlet - This is a UI class for handling connection and dragging behaviors, it also draws the Inlet/Outlet itself
class Plug    : public Component,
                public DragAndDropTarget,
                public SettableTooltipClient
{
public:
    /// This Class may exist in one of two modes
    enum Mode { Inlet, Outlet };
    
    /// An Event generated by a Plug
    struct Event {
        virtual ~Event() { ptr.release(); };
        
        enum Type {
            MouseDown,
            MouseUp,
            Drag,
            Connect,
            Disconnect
        } type;
        
        Event(Type type): type(type), ptr(this){}
        Event(const Event& event): type(event.type), ptr(this){}
        
        bool is(Type type) const {return this->type == type;}
        
        template<class T>
        const T* as(Type type) const {
            if (this->is(type))
                return static_cast<T*>(ptr.get());
            else
                return nullptr;
        }
    private:
        std::unique_ptr<Event> ptr;
    };
    
    struct MouseDown : public Event {
        MouseDown(Mode mode, PlugID plugID): Event(Type::MouseDown), mode(mode), plugID(plugID){}
        Mode mode; PlugID plugID;
    };
    struct MouseUp : public Event {
        MouseUp(): Event(Type::MouseUp){};
    };
    struct Drag : public Event {
        Drag(): Event(Type::Drag){};
    };
    struct Connect : public Event {
        Connect(PlugID source, PlugID destination): Event(Type::Connect), source(source), destination(destination){}
        PlugID source, destination;
    };
    struct Disconnect : public Event {
        Disconnect(): Event(Type::Disconnect){};
    }; // not implemented yet
    
    
    class Listener {
    public:
        virtual ~Listener() = default;
        virtual void onPlugEvent(const Event& event) = 0;
    };
   
    // These methods are used by objects that want to subscribe and unsubscribe from changes that occur in this plug
    void addListener(Listener *l) {listeners.add(l);}
    void removeListener(Listener *l) {listeners.remove(l);}
    
    /// To construct a plug you must specify a mode and a name
    Plug(Mode, const String&);
    
    ~Plug() {};

    void paint (Graphics&) override;
    void resized() override;
    
    /// Sets the ID that the Plug will use when broadcasting (assigned on module creation)
    void setId(uint64);
    
    String getName() {
        return name;
    }
    
    Mode getMode() {
        return mode;
    }
    
    PlugID getID() {
        return plugID;
    }
    
    Mode getOppositeMode() {
        return mode == Mode::Inlet ? Mode::Outlet : Mode::Inlet;
    }

private:
    typedef std::pair<Colour, Colour> ColourPair;
    typedef std::pair<String, String> StringPair;
    
    /// The plug's name - to be displayed or hinted
    String name;
    
    /// The plug's name - `Inlet` or `Outlet`
    Mode mode;
    
    /// The ID of this plug, containing the moduleID and plugID, that the Plug will use when broadcasting
    PlugID plugID;
    
    /// The Plug's colors - {inner, outer}
    ColourPair colors;
    
    /// This Plug's bounds
    Rectangle<float> bounds;
    /** The bounds for the Plug Name (if one is displayed)
     @see PlugOptions::drawName
     */
    Rectangle<int> nameBounds;
    
    /// A justification object to use when drawing the Name text
    Justification::Flags nameJustification;
    
    juce::ListenerList<Listener> listeners;
    
    bool canFitText;
    
//    /// Returns this Plug's ID as a String (for broadcasting) -> "moduleID>plugID"
//    String getStringId();
    
    void mouseDown(const MouseEvent&) override;
    void mouseUp(const MouseEvent&) override;
    void mouseDrag(const MouseEvent&) override;
    
    /** Callback for claiming interest in the thing being dragged.
     An inlet is interested in a dragged outlet but not another inlet,
     this also returns false if the drag source comes from the same module this Plug belongs to
     */
    bool isInterestedInDragSource (const SourceDetails&) override;
    /// Called when a drag source is dropped here and interest was claimed in isInterestedInDragSource()
    void itemDropped (const SourceDetails&) override;
    
    template<class T> void emitEvent(const T event) {
         // call the onPlugEvent method of every listener that has subscribed to be notified
         // pass in the value so that listeners can use it however they please
          listeners.call([event](Listener &l){ l.onPlugEvent(event); });
    }
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Plug)
};

/// A Plug in Inlet mode (other functionality might be added here)
class Inlet : public Plug
{
public:
    explicit Inlet(const String& name) : Plug(Plug::Inlet, name)
    {}
};

/// A Plug in Outlet mode (other functionality might be added here)
class Outlet : public Plug
{
public:
    explicit Outlet(const String& name) : Plug(Plug::Outlet, name)
    {}
};