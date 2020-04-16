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

//==============================================================================
/// The patch cord handler and drawer
class Connections : public Component,
                    public ActionListener,
                    public ChangeBroadcaster
{
//==============================================================================
public:
    /// The type for a unique identifier for an inlet or outlet (moduleID, inlet/outletID)
    typedef std::pair<uint32, int> IOid;
    /// A connection holds an inlet and an outlet (outlet, inlet)
    typedef std::pair<IOid, IOid> Connection;
    
    Connections();
    ~Connections();
    
    void paint (Graphics&) override;
    void resized () override;
    
    /// Registers an inlet with the Connections component, making it patchable
    IOid registerInlet (uint32, Component*);
    /// Registers an outlet with the Connections component, making it patchable
    IOid registerOutlet (uint32, Component*);
    
    /// Returns all existing connections as an Array of IOid pairs (outlet, inlet)
    Array<std::pair<IOid, IOid>> getAllConnectionIdPairs();
    
    /// Removes a module and unregisters all its inlets and outlets given its nodeID
    void removeModule(uint32);
    
    /// Chooses between two drawing styles for the connections and updates them
    void togglePatchCordType(bool);
    
//==============================================================================
private:
    /// This class holds the information on the inlets and outlets that currently exist in the patcher in the form of two 2D hash-maps and functions to add & remove entries.
    class IdStore
    {
    public:
        /// The inlets map holds phi_Inlet* and is accessed by two keys: nodeID & inletID
        std::map<uint32, std::map<int, Component*>> inlets;
        /// The inlets map holds phi_Outlet* and is accessed by two keys: nodeID & outletID
        std::map<uint32, std::map<int, Component*>> outlets;
        
        /// Adds an entry to inlets and returns the resulting unique identifier
        IOid storeInlet (uint32 nodeId, Component* inlet)
        {
            int inletId = getNewInletId(nodeId);
            inlets[nodeId][inletId] = inlet;
            return IOid(nodeId, inletId);
        }
        
        /// Adds an entry to outlets and returns the resulting unique identifier
        IOid storeOutlet (uint32 nodeId, Component* outlet)
        {
            int outletId = getNewOutletId(nodeId);
            outlets[nodeId][outletId] = outlet;
            return IOid(nodeId, outletId);
        }
        
        /// Generates a new ID for an inlet, given a nodeID
        int getNewInletId (const uint32 nodeId)
        {
            if (inlets.find(nodeId) == inlets.end()) return 0;
            return inlets[nodeId].rbegin()->first + 1;
        }
        
        /// Generates a new ID for an outlet, given a nodeID
        int getNewOutletId (const uint32 nodeId)
        {
            if (outlets.find(nodeId) == outlets.end()) return 0;
            return outlets[nodeId].rbegin()->first + 1;
        }
        
        /// Unregisters all inlets and outlets, given a nodeID
        void removeModule (const uint32 nodeId)
        {
            inlets.erase(nodeId);
            outlets.erase(nodeId);
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
    
    /// Fetches the center position (relative to this component) of an inlet
    Point<float> getInletCenterPositionFromId (const IOid);
    /// Fetches the center position (relative to this component) of an outlet
    Point<float> getOutletCenterPositionFromId (const IOid);
    
    /// Converts an action message ID string into an IOid type
    IOid stringToIOid (const String&);
    
    /// Adds an entry to the connections Array and notifies ChangeListeners
    void createConnection  (const IOid, const IOid);

    /// Receives action messages from inlets, outlets and module boxes, in order to create and update connections
    void actionListenerCallback (const String& ) override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Connections)
};
