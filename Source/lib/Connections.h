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

private:
    
    static void updateConnectionPath (Path&, Point<float>, Point<float>);
    
    class IdStore
    {
        
        template <class T> struct Entry
        {
            Entry(long key, T value) :
            key(key),
            value(value)
            {}
            
            long key;
            T value;
        };
        
    public:
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
    
    class Connection
    {
    public:
        Connection(String& inletId, String& outletId, Point<float> inletPosition, Point<float> outletPosition) :
        inletId(inletId),
        outletId(outletId),
        inletPosition(inletPosition),
        outletPosition(outletPosition)
        {
            updateConnectionPath(path, inletPosition, outletPosition);
        }
        
        String inletId;
        String outletId;
        Point<float> inletPosition;
        Point<float> outletPosition;
        Path path;
        
        bool isInletBeingDragged = false;
        bool isOutletBeingDragged = false;
        
        void updateInlet (Point<float> position)
        {
            updateConnectionPath(path, position, outletPosition);
        }
        
        void updateOutlet (Point<float> position)
        {
            updateConnectionPath(path, inletPosition, position);
        }
        
    };
    
    
    Point<float> getInletCenterPositionFromString (String&);
    Point<float> getOutletCenterPositionFromString (String&);
    
    void updateAllConnectionPaths ();
    
    OwnedArray<Connection> connections;
    
    Path dragPath;
    Point<float> dragPathAnchor;
    
    void actionListenerCallback (const String& ) override;
    
    void startConnect(Point<float>, Point<float>, bool, bool);
    void startInletConnect (Point<float>);
    void startOutletConnect (Point<float>);
    
    void createConnection  (String&, String&);
    
    Path getConnectionPath (Connection*);

    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Connections)
};
