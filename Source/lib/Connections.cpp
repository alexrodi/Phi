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

long registerTo (OwnedArray<long>& idArray)
{
    long* id = new long(0);
    
    if (idArray.size())
    {
        *id = *idArray.getLast() + 1;
    }
    
    return *idArray.add(id);
}

long Connections::registerInlet ()
{
    return registerTo (inletIDs);
}

long Connections::registerOutlet ()
{
    return registerTo (outletIDs);
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

Point<float> getTopRightFromString (String& coordString)
{
    Rectangle<int> ioBounds;
    return ioBounds.fromString(coordString.removeCharacters(",")).getTopRight().toFloat();
}

void Connections::actionListenerCallback (const String& message)
{
    
    //std::cout << message << std::endl;
    
    // Here we receive all clicks from all inlets and outlets
    if (message.containsWholeWord ("mouseDown"))
    {
        String coordString = message.fromFirstOccurrenceOf("mouseDown", false, false);
        Point<float> ioPosition = getTopRightFromString (coordString);

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
        Connection* newConnection = connections.getLast();
        if (newConnection->isInletBeingDragged || newConnection->isOutletBeingDragged)
        {
            connections.removeLast();
            repaint();
        }
    }
    else if (message.containsWholeWord ("draggingPatchCord"))
    {
        mousePoint = getMouseXYRelative().toFloat();
        repaint();
    }
    else if (message.containsWholeWord ("dropped"))
    {
        String coordString = message.fromFirstOccurrenceOf("dropped", false, false);
        Point<float> ioPosition = getTopRightFromString (coordString);
        
        finishConnect(ioPosition);
        repaint();
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

void Connections::startInletConnect (Point<float> inletPosition)
{
    startConnect(inletPosition, mousePoint, false, true);
}
void Connections::startOutletConnect (Point<float> outletPosition)
{
    startConnect(mousePoint, outletPosition, true, false);
}

void Connections::finishConnect(Point<float> ioPosition)
{
    Connection* newConnection = connections.getLast();
    
    if (newConnection->isInletBeingDragged)
    {
        newConnection->inletPosition = ioPosition;
    }
    else if (newConnection->isOutletBeingDragged)
    {
        newConnection->outletPosition = ioPosition;
    }
    
    newConnection->isInletBeingDragged = false;
    newConnection->isOutletBeingDragged = false;
    newConnection->path.clear();
    newConnection->path.startNewSubPath (newConnection->inletPosition);
    newConnection->path.lineTo(newConnection->outletPosition);
    newConnection->path.closeSubPath();
}

