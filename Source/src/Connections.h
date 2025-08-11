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
#include "PhiColours.h"
#include "Plug.h"

//==============================================================================
/// A connection holds an inlet and an outlet (outlet, inlet)
struct PhiConnection : AudioProcessorGraph::Connection
{
    Path path;
    Colour colour;
    
    using AudioProcessorGraph::Connection::Connection;
    
    PlugID sourcePlug() { return source; }
    PlugID destinationPlug() { return destination; }
};

/// The connection manager and drawer
class Connections : public Component,
                    public ChangeBroadcaster,
                    public ChangeListener,
                    public PlugHandler,
                    public LassoSource<PhiConnection*>
{
//==============================================================================
public:
    Connections();
    ~Connections();
    
    void paint (Graphics&) override;
    void resized () override;
    
    /// Registers an inlet or outlet with the Connections component, making it patchable
    PlugID registerPlug (uint32, const Plug&);
    
    /// Returns all existing connections as an Array of PlugID pairs (outlet, inlet)
    const std::vector<PhiConnection>& getConnections();
    
    /// Removes a module and unregisters all its inlets and outlets given its nodeID
    void removeModule(uint32);
    
    /// Chooses between two drawing styles for the connections and updates them
    void togglePatchCordType(bool);
    
    void mouseDown(const MouseEvent& e) override;
    void mouseMove(const MouseEvent& e) override;
    
    bool onMouseRightButton(const MouseEvent& e);
    
    void deselectAll();
    
    void refresh();
    
    /// Listener for key presses
    bool keyPressed (const KeyPress& key) override;
    
    class HiddenLasso: public LassoComponent<PhiConnection*> {
        void paint(Graphics& g) {}
    } lasso;
    
//==============================================================================
private:
    /// This class holds the information on the inlets and outlets that currently exist in the patcher in the form of two 2D hash-maps and functions to add & remove entries.
    class IdStore
    {
        typedef std::map<uint32, std::map<int, const Plug*>> PlugMap;
    public:
        /// The inlets map holds Inlet* and is accessed by two keys: nodeID & inletID
        PlugMap inlets;
        /// The inlets map holds Outlet* and is accessed by two keys: nodeID & outletID
        PlugMap outlets;
        
        /// Adds an entry to inlets or outlets and returns the resulting unique identifier
        PlugID storePlug (uint32 moduleID, const Plug& plug)
        {
            int plugId = newPlugId(moduleID, plug);
            mapOfMode(plug.getMode())[moduleID][plugId] = &plug;
            return {moduleID, plugId};
        }
        
        /// Gets a stored inlet or outlet
        const Plug& getPlug (PlugType plugType, PlugID plugID)
        {
            return *mapOfMode(plugType)[plugID.moduleID()][plugID.plugID()];
        }
        
        /// Generates a new ID for an inlet or outlet, given a nodeID
        int newPlugId (uint32 moduleID, const Plug& plug)
        {
            auto plugs = mapOfMode(plug.getMode());
            if (plugs.find(moduleID) == plugs.end()) return 0;
            return plugs[moduleID].rbegin()->first + 1;
        }
        
        PlugMap& mapOfMode(PlugType mode)
        {
            return mode == PlugType::Inlet ? inlets : outlets;
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
    PathStrokeType strokeType = PathStrokeType(5.0f, PathStrokeType::JointStyle::mitered, PathStrokeType::EndCapStyle::rounded);
    
    /// All the existing connections are stored in this Array
    std::vector<PhiConnection> connections;
    
    /// A solo path to use when dragging connections
    Path dragPath;
    /// The point used to anchor the dragging patch cord, the other point is assumed to be the mouse
    Point<float> dragPathAnchor;
    
    /// The curretly used patch cord drawing routine
    std::function<Path(Point<float>,Point<float>)> getConnectionPath;
    
    /// The list of selected connections
    SelectedItemSet<PhiConnection*> selectedConnections;

    //============================================================
    
    void updateDragPath();
    
    /// Forces an update of all patch cords, evaulating all inlet/outlet positions
    void updateAllConnectionPaths ();
    
    void updateConnectionPath (PhiConnection& connection);
    
    void removeConnectionsIf(std::function<bool(PhiConnection&)> predicate);
    
    void deleteAllSelectedConnections();
  
    /// Finds the middle point between two points with an optional weight factor
    static Point<float> getMiddlePoint (const Point<float>, const Point<float>, bool);
    /// A callback for drawing patch cords that applies a vertical weight
    static Path patchCordTypeACallback (const Point<float>, const Point<float>);
    /// A callback for drawing patch cords with a horizontal S shape
    static Path patchCordTypeBCallback (const Point<float>, const Point<float>);
    
    /// Fetches the center position (relative to this component) of an inlet or outlet
    Point<float> getPlugCenterPositionFromId (PlugType, const PlugID);
    
    /// Adds an entry to the connections Array and notifies ChangeListeners
    void createConnection  (const PhiConnection&);
    
    bool containsConnectionWith (const PhiConnection&);
    
    void openColourSelector(Point<int> pointTo, Colour initialColour);
    
    void onConnectionStart (PlugType, PlugID) override;
    void onConnectionEnd (PlugID, PlugID) override;
    void onConnectionDrag () override;
    void onConnectionRelease () override;
    
    void changeListenerCallback (ChangeBroadcaster* source) override;
    
    void findLassoItemsInArea (Array<PhiConnection*>& itemsFound, const Rectangle<int>& area) override;
    SelectedItemSet<PhiConnection*>& getLassoSelection() override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Connections)
};
