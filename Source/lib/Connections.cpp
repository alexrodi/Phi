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
/**
     This whole file needs rethinking, the connections are being stored, but I'm not convinced on the method.
      - The action messages are strings which need parsing
      - The Inlets and outlets still cannot detect mouse-ups (then the mouse down was somewhere else)
      - There seem to be drag-and-drop systems which could fit a lot better in doing this job
       (it's essentially the same system but we should draw a patch cord instead of a ghost version of a UI element)
 
 */

Connections::Connections()
{
    setPaintingIsUnclipped(true);
    setRepaintsOnMouseActivity(true);
    setInterceptsMouseClicks(false, false);
}

Connections::~Connections()
{
}

void Connections::paint (Graphics& g)
{
    
    if (connections.size())
    {
        g.setColour (Colours::red);
        for (Connection* connection : connections)
        {
            if (connection->isInletBeingDragged)
            {
    //            std::cout << "mouse " << mousePoint.toString() << std::endl;
    //            std::cout << "inlet " << newConnection->inletPosition.toString() << std::endl;
                connection->path.clear();
                connection->path.startNewSubPath (connection->outletPosition);
                connection->path.lineTo(mousePoint);
                connection->path.closeSubPath();
            }
            else if (connection->isOutletBeingDragged)
            {
                connection->path.clear();
                connection->path.startNewSubPath (mousePoint);
                connection->path.lineTo(connection->inletPosition);
                connection->path.closeSubPath();
            }
            g.strokePath (connection->path, PathStrokeType (2.0f));
        }
    }
}


void Connections::resized ()
{
}

//void Connections::mouseDrag (const MouseEvent& e)
//{
//    std::cout << e.x << e.y << std::endl;
//}
//
//bool Connections::hitTest (int x, int y)
//{
//    if (connections.size())
//    {
//        Connection* newConnection = connections.getLast();
//        Point<float> clickedPoint(x, y);
//        Point<float> clickedPointOnPath;
//
//        if (newConnection->path.getNearestPoint(clickedPoint, clickedPointOnPath) < 10)
//            return true;
//    }

//    return false;
//}


void Connections::actionListenerCallback (const String& message)
{
    
    // Here we receive all clicks from all inlets and outlets
    if (message.containsWholeWord ("mouseDown"))
    {
        String coordString = message.fromFirstOccurrenceOf("mouseDown", false, false);
        Rectangle<int> ioBounds;
        Point<float> ioPosition = ioBounds.fromString(coordString.removeCharacters(",")).getTopRight().toFloat();
        
        if (message.containsWholeWord ("inlet"))
        {
            startInletConnect(ioPosition);
        }
        else if (message.containsWholeWord ("outlet"))
        {
            startOutletConnect(ioPosition);
        }
    }
    else if (message.containsWholeWord ("mouseUp"))
    {
        if (message.containsWholeWord ("inlet"))
        {
               finishInletConnect();
        }
        else if (message.containsWholeWord ("outlet"))
        {
               finishOutletConnect();
        }
    }
    else if (message.containsWholeWord ("mouseDrag"))
    {
        String coordString = message.fromFirstOccurrenceOf("mouseDrag ", false, false);
        Rectangle<int> mouseBounds;
        
        mousePoint = mouseBounds.fromString(coordString.removeCharacters(",")).getTopRight().toFloat();
        
        repaint();
        toFront(true);
    }
}

void Connections::startInletConnect (Point<float> inletPosition)
{
    connections.add(new Connection);
    Connection* newConnection = connections.getLast();
    newConnection->isOutletBeingDragged = true;
    newConnection->inletPosition = inletPosition;
}
void Connections::startOutletConnect (Point<float> outletPosition)
{
    connections.add(new Connection);
    Connection* newConnection = connections.getLast();
    newConnection->isInletBeingDragged = true;
    newConnection->outletPosition = outletPosition;
}

void Connections::finishInletConnect ()
{
    Connection* newConnection = connections.getLast();
    newConnection->isOutletBeingDragged = false;
    newConnection->path.clear();
    newConnection->path.startNewSubPath (newConnection->inletPosition);
    newConnection->outletPosition = mousePoint;
    newConnection->path.lineTo(newConnection->outletPosition);
    newConnection->path.closeSubPath();
}
void Connections::finishOutletConnect ()
{
    Connection* newConnection = connections.getLast();
    newConnection->isInletBeingDragged = false;
    newConnection->path.clear();
    newConnection->path.startNewSubPath (newConnection->outletPosition);
    newConnection->inletPosition = mousePoint;
    newConnection->path.lineTo(newConnection->inletPosition);
    newConnection->path.closeSubPath();
}
