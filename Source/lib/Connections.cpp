/*
  ==============================================================================

    Connections.cpp
    Created: 20 Feb 2020 8:18:35pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#include <JuceHeader.h>
#include "Connections.h"

//==============================================================================

Connections::Connections()
{
    setAlwaysOnTop(true);
    setPaintingIsUnclipped(true);
    setInterceptsMouseClicks(false, false);
}

Connections::~Connections()
{
}

void Connections::paint (Graphics& g)
{
    if(! dragPath.isEmpty())
    {
        g.strokePath ( dragPath, PathStrokeType (2.0f) );
    }
    
    if (connections.size())
    {
        g.setColour (Colours::red);
        for (Connection* connection : connections)
        {
            g.strokePath ( getConnectionPath(connection), PathStrokeType (2.0f) );
        }
    }
}

Path Connections::getConnectionPath (Connection* connection)
{
    if (connection->isInletBeingDragged)
    {
        connection->updateInlet(getMouseXYRelative().toFloat());
    }
    else if (connection->isOutletBeingDragged)
    {
        connection->updateOutlet(getMouseXYRelative().toFloat());
    }
    return connection->path;
}


void Connections::resized ()
{
}

long Connections::registerInlet (phi_Inlet* inlet)
{
    long idNumber = idStore.getNewInletId();
    idStore.storeInlet(idNumber, inlet);
    inlet->inletID = idNumber;
    return idNumber;
}

long Connections::registerOutlet (phi_Outlet* outlet)
{
    long idNumber = idStore.getNewOutletId();
    idStore.storeOutlet(idNumber, outlet);
    outlet->outletID = idNumber;
    return idNumber;
}

Point<float> getTopRightFromString (String& coordString)
{
    Rectangle<int> ioBounds;
    return ioBounds.fromString(coordString.removeCharacters(",")).getTopRight().toFloat();
}

Point<float> Connections::getInletCenterPositionFromString (String& inletId)
{
    Component* inlet = idStore.inlets[inletId.toUTF8().getIntValue32()];
    
    return getLocalPoint(inlet, inlet->getLocalBounds().getCentre().toFloat()) ;
}

Point<float> Connections::getOutletCenterPositionFromString (String& outletId)
{
    Component* outlet = idStore.outlets[outletId.toUTF8().getIntValue32()];
    
    return getLocalPoint(outlet, outlet->getLocalBounds().getCentre().toFloat()) ;
}

void Connections::actionListenerCallback (const String& message)
{
    
    std::cout << message << std::endl;
    
    // Here we receive all clicks from all inlets and outlets
    if (message.containsWholeWord ("mouseDown"))
    {
        String coordString = message.fromFirstOccurrenceOf("mouseDown", false, false);
        Point<float> ioPosition = getTopRightFromString (coordString);
        
        dragPathAnchor = ioPosition;
    }
    else if (message.containsWholeWord ("dragging"))
    {
        updatedragPath(dragPathAnchor, getMouseXYRelative().toFloat());
        repaint();
    }
    else if (message.containsWholeWord ("mouseUp"))
    {
        dragPath.clear();
        repaint();
    }
    else if (message.containsWholeWord ("connect"))
    {
        String inletId = message.fromFirstOccurrenceOf("connect ", false, false).upToFirstOccurrenceOf("&", false, false);
        String outletId = message.fromFirstOccurrenceOf("&", false, false);
        
        createConnection(outletId, inletId);
        repaint();
    }
}

void Connections::createConnection(String& outletId, String& inletId)
{
    connections.add( new Connection( inletId
                                    , outletId
                                    , getInletCenterPositionFromString(inletId)
                                    , getOutletCenterPositionFromString(outletId)));
    
}


void Connections::updatedragPath(Point<float> positionA, Point<float> positionB)
{
    dragPath.clear();
    dragPath.startNewSubPath (positionA);
    dragPath.lineTo (positionB);
    dragPath.closeSubPath();
}
