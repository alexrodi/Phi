/*
  ==============================================================================

    Connections.h
    Created: 20 Feb 2020 8:18:35pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

///@cond
#include <JuceHeader.h>
///@endcond
#include "Plug.h"

//==============================================================================
/// The patch cord handler and drawer
class Connections : public Component,
                    public ActionListener,
                    public Plug::Listener,
                    public ChangeBroadcaster
{
//==============================================================================
public:
    /// A connection holds an inlet and an outlet (outlet, inlet)
    struct Connection
    {
        Connection(){}
        
        Connection(PlugID source, PlugID destination):
        source(source),
        destination(destination)
        {}
        
        Connection(const Connection& other):
        source(other.source),
        destination(other.destination)
        {}
        
        bool operator== (const Connection& other) const noexcept
        {
            return other.source == source && other.destination == destination;
        }
    
        PlugID source, destination;
    };
    
    Connections();
    ~Connections();
    
    void paint (Graphics&) override;
    void resized () override;
    
    /// Registers an inlet or outlet with the Connections component, making it patchable
    PlugID registerPlug (uint32, Plug*);
    
    /// Returns all existing connections as an Array of PlugID pairs (outlet, inlet)
    Array<Connection> getAllConnectionIdPairs();
    
    /// Removes a module and unregisters all its inlets and outlets given its nodeID
    void removeModule(uint32);
    
    /// Chooses between two drawing styles for the connections and updates them
    void togglePatchCordType(bool);
    
//==============================================================================
private:
    /// This class holds the information on the inlets and outlets that currently exist in the patcher in the form of two 2D hash-maps and functions to add & remove entries.
    class IdStore
    {
        typedef std::map<uint32, std::map<uint32, Plug*>> PlugMap;
    public:
        /// The inlets map holds Inlet* and is accessed by two keys: nodeID & inletID
        PlugMap inlets;
        /// The inlets map holds Outlet* and is accessed by two keys: nodeID & outletID
        PlugMap outlets;
        
        /// Adds an entry to inlets or outlets and returns the resulting unique identifier
        PlugID storePlug (uint32 moduleID, Plug* plug)
        {
            uint32 plugId = newPlugId(moduleID, *plug);
            mapOfMode(plug->getMode())[moduleID][plugId] = plug;
            return PlugID(std::pair(moduleID, plugId));
        }
        
        /// Gets a stored inlet or outlet
        Plug* getPlug (Plug::Mode plugMode, PlugID plugID)
        {
            auto module = plugID.moduleID();
            auto plug = plugID.plugID();
            return mapOfMode(plugMode)[plugID.moduleID()][plugID.plugID()];
        }
        
        /// Generates a new ID for an inlet or outlet, given a nodeID
        uint32 newPlugId (uint32 moduleID, Plug& plug)
        {
            auto plugs = mapOfMode(plug.getMode());
            if (plugs.find(moduleID) == plugs.end()) return 0;
            return plugs[moduleID].rbegin()->first + 1;
        }
        
        PlugMap& mapOfMode(Plug::Mode mode)
        {
            return mode == Plug::Mode::Inlet ? inlets : outlets;
        }
        
        /// Unregisters all inlets and outlets, given a nodeID
        void removeModule (const uint32 moduleID)
        {
            inlets.erase(moduleID);
            outlets.erase(moduleID);
        }
        
    } idStore;
    //============================================================
    
    static constexpr float CORD_WEIGHT = 0.2;
    static constexpr float CORD_THICKNESS = 5.0;
    
    /// All the existing connections are stored in this Array
    Array<Connection> connections;
    
    /// A solo path to use when dragging connections
    Path dragPath;
    /// The point used to anchor the dragging patch cord, the other point is assumed to be the mouse
    Point<float> dragPathAnchor;
    
    /// All the existing connections are concatenated into this single path to be drawn
    Path allConnectionsPath;
    /// The curretly used patch cord drawing routine
    std::function<Path(Point<float>,Point<float>)> getConnectionPath;

    //============================================================
    
    /// Forces an update of all patch cords, evaulating all inlet/outlet positions
    void updateAllConnectionPaths ();
  
    /// Finds the middle point between two points with an optional weight factor
    static Point<float> getMiddlePoint (const Point<float>, const Point<float>, bool);
    /// A callback for drawing patch cords that applies a vertical weight
    static Path patchCordTypeACallback (const Point<float>, const Point<float>);
    /// A callback for drawing patch cords with a horizontal S shape
    static Path patchCordTypeBCallback (const Point<float>, const Point<float>);
    
    /// Fetches the center position (relative to this component) of an inlet or outlet
    Point<float> getPlugCenterPositionFromId (Plug::Mode, const PlugID);
    
    /// Adds an entry to the connections Array and notifies ChangeListeners
    void createConnection  (const Connection);

    /// Receives action messages from inlets, outlets and module boxes, in order to create and update connections
    void actionListenerCallback (const String& ) override;
    
    void onPlugEvent (const Plug::Event&) override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Connections)
};
