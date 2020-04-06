/*
  ==============================================================================

    Connections.h
    Created: 20 Feb 2020 8:18:35pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "phi_Inlet.h"
#include "phi_Outlet.h"

//==============================================================================
/*
*/

#define CORD_WEIGHT 0.2f
#define CORD_THICKNESS 5.0f

class Connections : public Component,
                    public ActionListener,
                    public ChangeBroadcaster
{
public:
    typedef std::pair<uint32, int> IOid;
    
    Connections();
    ~Connections();
    
    void paint (Graphics&) override;
    void resized () override;
    
    std::pair<uint32, int> registerInlet (uint32, phi_Inlet*);
    std::pair<uint32, int> registerOutlet (uint32, phi_Outlet*);
    
    Array<std::pair<IOid, IOid>> getAllConnectionIdPairs();
    
    void removeModule(uint32);
    
    void togglePatchCordType();

private:
    typedef std::function<void(Path&,Point<float>,Point<float>)> ConnectionPathCallback;
    
    struct IdStore
    {
        /*
        IdStore is a 2D Map (e.g. inlets):
        __________________________
  key   | module (uint32 nodeId) |
        -------------------------------------------
 value  |     inletId (long)     |   phi_Inlet*   |
 value  |     inletId (long)     |   phi_Inlet*   |
        -------------------------------------------
                   key                 value
        */
        
        std::map<uint32, std::map<int, phi_Inlet*>> inlets;
        std::map<uint32, std::map<int, phi_Outlet*>> outlets;
        
        IOid storeInlet (uint32 nodeId, phi_Inlet* inlet)
        {
            int inletId = getNewInletId(nodeId);
            inlets[nodeId][inletId] = inlet;
            return IOid(nodeId, inletId);
        }
        
        IOid storeOutlet (uint32 nodeId, phi_Outlet* outlet)
        {
            int outletId = getNewOutletId(nodeId);
            outlets[nodeId][outletId] = outlet;
            return IOid(nodeId, outletId);
        }
        
        int getNewInletId (const uint32 nodeId)
        {
            if (inlets.find(nodeId) == inlets.end()) return 0;
            return inlets[nodeId].rbegin()->first + 1;
        }
        
        int getNewOutletId (const uint32 nodeId)
        {
            if (outlets.find(nodeId) == outlets.end()) return 0;
            return outlets[nodeId].rbegin()->first + 1;
        }
        
        void removeModule (const uint32 nodeId)
        {
            inlets.erase(nodeId);
            outlets.erase(nodeId);
        }
        
    } idStore;
    
    class Connection
    {
    public:
        Connection(ConnectionPathCallback& updateConnectionPathCallback
                   , const IOid inletId
                   , const IOid outletId
                   , Point<float> inletPosition
                   , Point<float> outletPosition) :
        inletId(inletId),
        outletId(outletId),
        inletPosition(inletPosition),
        outletPosition(outletPosition)
        {
            updateConnectionPathCallback(path, inletPosition, outletPosition);
        }
        
        IOid inletId;
        IOid outletId;
        Point<float> inletPosition;
        Point<float> outletPosition;
        Path path;
        
        ConnectionPathCallback updateConnectionPathCallback;
        
        bool isInletBeingDragged = false;
        bool isOutletBeingDragged = false;
        
        void updateInlet (Point<float> position)
        {
            updateConnectionPathCallback(path, position, outletPosition);
        }
        
        void updateOutlet (Point<float> position)
        {
            updateConnectionPathCallback(path, inletPosition, position);
        }
        
    };
    
    Path allConnectionsPath;
    bool patchCordTypeToggle;
    ConnectionPathCallback updateConnectionPath;
    
    void updateAllConnectionPaths ();
  
    static void patchCordTypeACallback (Path&, Point<float>, Point<float>);
    static void patchCordTypeBCallback (Path&, Point<float>, Point<float>);
    
    Point<float> getInletCenterPositionFromId (const IOid);
    Point<float> getOutletCenterPositionFromId (const IOid);
    
    OwnedArray<Connection> connections;
    
    Path dragPath;
    Point<float> dragPathAnchor;
    
    IOid stringToIOid (const String&);
    
    const bool hasConnectionWithIds (const IOid, const IOid);
       
    void createConnection  (const IOid, const IOid);
    
    Path getConnectionPath (Connection*);

    void actionListenerCallback (const String& ) override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Connections)
};
