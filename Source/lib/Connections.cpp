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
        
        Connection* newConnection = connections.getLast();
        newConnection->path.clear();
        
        if (newConnection->isInletBeingDragged)
        {
//            std::cout << "mouse " << mousePoint.toString() << std::endl;
//            std::cout << "inlet " << newConnection->inletPosition.toString() << std::endl;
            newConnection->path.startNewSubPath (newConnection->outletPosition);
            newConnection->path.lineTo(mousePoint);
        }
        else if (newConnection->isOutletBeingDragged)
        {
            newConnection->path.startNewSubPath (mousePoint);
            newConnection->path.lineTo(newConnection->inletPosition);
        }
        g.strokePath (newConnection->path, PathStrokeType (2.0f));
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
    std::cout << "finished inlet!"<< std::endl;
    Connection* newConnection = connections.getLast();
    newConnection->isInletBeingDragged = false;
}
void Connections::finishOutletConnect ()
{
    std::cout << "finished outlet!"<< std::endl;
    Connection* newConnection = connections.getLast();
    newConnection->isOutletBeingDragged = false;
}
