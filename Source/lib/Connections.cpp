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
      - The whole system is action-based and sequential which makes it cumbersome to combine messages
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
            g.strokePath ( getConnectionPath(connection), PathStrokeType (2.0f) );
        }
    }
}

Path Connections::getConnectionPath (Connection* connection)
{
    if (connection->isInletBeingDragged || connection->isOutletBeingDragged)
    {
        connection->path.clear();
        connection->path.startNewSubPath (connection->isInletBeingDragged ? mousePoint : connection->inletPosition);
        connection->path.lineTo(connection->isOutletBeingDragged ? mousePoint : connection->outletPosition);
        connection->path.closeSubPath();
    }
    return connection->path;
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
    
    std::cout << message << std::endl;
    
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
        if (message.containsWholeWord ("inlet") || message.containsWholeWord ("outlet"))
        {
            finishConnect();
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

void Connections::startConnect(Point<float> inletPosition, Point<float> outletPosition,
                               bool isInletBeingDragged, bool isOutletBeingDragged)
{
    connections.add(new Connection);
    Connection* newConnection = connections.getLast();
    newConnection->inletPosition = inletPosition;
    newConnection->outletPosition = outletPosition;
    newConnection->isInletBeingDragged = isInletBeingDragged;
    newConnection->isOutletBeingDragged = isOutletBeingDragged;
}

void Connections::finishConnect()
{
    Connection* newConnection = connections.getLast();
    
    if (newConnection->isInletBeingDragged)
    {
        newConnection->inletPosition = mousePoint;
    }
    else if (newConnection->isOutletBeingDragged)
    {
        newConnection->outletPosition = mousePoint;
    }
    
    newConnection->isInletBeingDragged = false;
    newConnection->isOutletBeingDragged = false;
    newConnection->path.clear();
    newConnection->path.startNewSubPath (newConnection->inletPosition);
    newConnection->path.lineTo(newConnection->outletPosition);
    newConnection->path.closeSubPath();
}

void Connections::startInletConnect (Point<float> inletPosition)
{
    startConnect(inletPosition, mousePoint, false, true);
}
void Connections::startOutletConnect (Point<float> outletPosition)
{
    startConnect(mousePoint, outletPosition, true, false);
}
