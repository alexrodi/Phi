/*
  ==============================================================================

    Connections.h
    Created: 20 Feb 2020 8:18:35pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "./UIobjects/phi_Inlet.h"
#include "./UIobjects/phi_Outlet.h"

//==============================================================================
/*
*/
class Connections : public Component,
                    public ActionListener
{
public:
    Connections();
    ~Connections();
    
    void paint (Graphics&) override;
    void resized () override;
    
    long registerInlet ();
    long registerOutlet ();

private:
    
    class Connection
    {
    public:
        Connection()
        {
            inletPosition.setXY(0.f,0.f);
            outletPosition.setXY(0.f,0.f);
        }
        Point<float> inletPosition;
        Point<float> outletPosition;
        Path path;
        
        bool isInletBeingDragged = false;
        bool isOutletBeingDragged = false;
    };
    
    OwnedArray<long> inletIDs;
    OwnedArray<long> outletIDs;
    
    OwnedArray<Connection> connections;
    
    void actionListenerCallback (const String& message) override;
    
    void startConnect(Point<float>, Point<float>, bool, bool);
    void startInletConnect (Point<float>);
    void startOutletConnect (Point<float>);
    void finishConnect(Point<float>);
    
    Path getConnectionPath (Connection*);
    
    //void mouseDrag (const MouseEvent& e) override;
    //bool hitTest (int x, int y) override;
    
    Point<float> mousePoint;

    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Connections)
};
