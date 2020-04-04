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
                    public ActionListener
{
public:
    Connections();
    ~Connections();
    
    void paint (Graphics&) override;
    void resized () override;
    
    long registerInlet (phi_Inlet*);
    long registerOutlet (phi_Outlet*);
    
    void togglePatchCordType();

private:
    
    typedef std::function<void(Path&,Point<float>,Point<float>)> ConnectionPathCallback;
    
    struct IdStore
    {
        std::map<long, phi_Inlet*> inlets;
        std::map<long, phi_Outlet*> outlets;
        
        void storeInlet (long entryId, phi_Inlet* entry)
        {
            inlets.insert_or_assign(entryId, entry);
        }
        
        void storeOutlet (long entryId, phi_Outlet* entry)
        {
            outlets.insert_or_assign(entryId, entry);
        }
        
        long getNewInletId ()
        {
            if (inlets.empty()) return 0;
            return inlets.rbegin()->first + 1;
        }
        
        long getNewOutletId ()
        {
            if (outlets.empty()) return 0;
            return outlets.rbegin()->first + 1;
        }
        
    } idStore;
    
    Path allConnectionsPath;
       void updateAllConnectionPaths ();
       
       bool patchCordTypeToggle;
       
       static void patchCordTypeACallback (Path&, Point<float>, Point<float>);
       static void patchCordTypeBCallback (Path&, Point<float>, Point<float>);
       ConnectionPathCallback updateConnectionPath;
    
    class Connection
    {
    public:
        Connection(ConnectionPathCallback& updateConnectionPathCallback
                   , const String& inletId
                   , const String& outletId
                   , Point<float> inletPosition
                   , Point<float> outletPosition) :
        inletId(inletId),
        outletId(outletId),
        inletPosition(inletPosition),
        outletPosition(outletPosition)
        {
            updateConnectionPathCallback(path, inletPosition, outletPosition);
        }
        
        String inletId;
        String outletId;
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
    
    
    Point<float> getInletCenterPositionFromString (const String&);
    Point<float> getOutletCenterPositionFromString (const String&);
    
    OwnedArray<Connection> connections;
    
    Path dragPath;
    Point<float> dragPathAnchor;
    
    const bool hasConnectionWithIds (const String&, const String&);
       
    void createConnection  (const String&, const String&);
    
    Path getConnectionPath (Connection*);

    void actionListenerCallback (const String& ) override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Connections)
};
