/*
  ==============================================================================

    Connections.cpp
    Created: 20 Feb 2020 8:18:35pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

///@cond
#include <JuceHeader.h>
///@endcond
#include "Connections.h"

//==============================================================================

Connections::Connections() :
getConnectionPath{patchCordTypeBCallback}
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
        g.setColour (Colours::grey.withAlpha(0.5f));
        g.strokePath ( dragPath, PathStrokeType (4.0f) );
    }
    
    if (connections.size())
    {
        g.setColour (Colours::grey);
        g.strokePath ( allConnectionsPath
                      , PathStrokeType ( CORD_THICKNESS
                                        , PathStrokeType::JointStyle::mitered
                                        , PathStrokeType::EndCapStyle::rounded )
                      );
    }
}

void Connections::resized ()
{
}

PlugID Connections::registerPlug (uint32 moduleId, Plug* plug)
{
    return idStore.storePlug(moduleId, plug);
}

Point<float> Connections::getPlugCenterPositionFromId (Plug::Mode plugMode, const PlugID plugID)
{
    auto plug = idStore.getPlug(plugMode, plugID);
    
    return getLocalPoint(&*plug, plug->getLocalBounds().getCentre().toFloat()) ;
}

void Connections::updateAllConnectionPaths ()
{
    allConnectionsPath.clear();
    for (Connection connection : connections)
    {
        allConnectionsPath.addPath (getConnectionPath (getPlugCenterPositionFromId(Plug::Mode::Inlet, connection.destination)
                                                       , getPlugCenterPositionFromId(Plug::Mode::Outlet, connection.source)));
    }
    repaint();
}

void Connections::actionListenerCallback (const String& message)
{
    if (message.containsWholeWord ("moduleChanged"))
    {
        updateAllConnectionPaths();
    }
}

void Connections::onPlugEvent (const std::unique_ptr<Plug::Event> event)
{
    // Here we receive events from inlets and outlets
    
    typedef Plug::Event::Type EventType;
    
    if (event->is(EventType::MouseDown)) {
        auto object = static_cast<const Plug::MouseDown*>(event.get());
        dragPathAnchor = getPlugCenterPositionFromId(object->mode, object->plugID);
    } else if (event->is(EventType::MouseUp)) {
        // When plugs get clicked they might bring the module to the front
        // this assures "Connections" always stays in front
        toFront(false);
        dragPath.clear();
        repaint();
    } else if (event->is(EventType::Drag)) {
        dragPath = getConnectionPath (dragPathAnchor, getMouseXYRelative().toFloat());
        repaint();
    } else if (event->is(EventType::Connect)) {
        auto object = static_cast<const Plug::Connect*>(event.get());
        createConnection (Connection(object->source, object->destination));
        updateAllConnectionPaths();
    } else if (event->is(EventType::Disconnect)) {
        // Not implemented
    }
    
    
//    struct MouseDown {Mode mode; PlugID plugID;};
//    struct MouseUp {};
//    struct Drag {};
//    struct Connect { PlugID source; PlugID destination; };
//    struct Disconnect {}; // not implemented yet
    
//    if (message.containsWholeWord ("mouseDown"))
//    {
//        PlugID plugId = stringToPlugID(message.fromFirstOccurrenceOf("#", false, false));
//
//        if (message.containsWholeWord ("inlet"))
//        {
//            dragPathAnchor = getPlugCenterPositionFromId(Inlet, plugId);
//        }
//        else if (message.containsWholeWord ("outlet"))
//        {
//            dragPathAnchor = getPlugCenterPositionFromId(Outlet, plugId);
//        }
//    }
//    else if (message.containsWholeWord ("dragging"))
//    {
//        dragPath = getConnectionPath (dragPathAnchor, getMouseXYRelative().toFloat());
//        repaint();
//    }
//    else if (message.containsWholeWord ("mouseUp"))
//    {
//        // When plugs get clicked they might bring the module to the front
//        // this assures "Connections" always stays in front
//        toFront(false);
//        dragPath.clear();
//        repaint();
//    }
//    else if (message.containsWholeWord ("connect"))
//    {
//        const String inletIdString = message.fromFirstOccurrenceOf("connect ", false, false).upToFirstOccurrenceOf("&", false, false);
//        const String outletIdString = message.fromFirstOccurrenceOf("&", false, false);
//
//        createConnection (stringToPlugID(inletIdString), stringToPlugID(outletIdString));
//        updateAllConnectionPaths();
//    }
}

void Connections::createConnection(const Connection connection)
{
    connections.addIfNotAlreadyThere( connection );
    sendChangeMessage(); // notify new connections
}

void Connections::removeModule(uint32 moduleId)
{
    int i = 0;
    while (i < connections.size())
    {
        if (connections[i].source.moduleID() == moduleId || connections[i].destination.moduleID() == moduleId)
        {
            connections.remove(i);
        }
        else i++;
    }
    idStore.removeModule(moduleId);
    updateAllConnectionPaths();
}

Array<Connections::Connection> Connections::getAllConnectionIdPairs()
{
    return connections;
}

void Connections::togglePatchCordType(bool toggle)
{
    getConnectionPath = toggle ? patchCordTypeACallback : patchCordTypeBCallback;
    updateAllConnectionPaths();
}

Point<float> Connections::getMiddlePoint (Point<float> point1, Point<float> point2, bool applyWeight = false)
{
    const float distance = point1.getDistanceFrom(point2);
    const Point<float> middlePoint = point1.getPointOnCircumference(distance * 0.5, point1.getAngleToPoint(point2));
    
    if (applyWeight)
        return middlePoint.translated(0.0f, distance * CORD_WEIGHT);
    
    return middlePoint;
}

Path Connections::patchCordTypeACallback (Point<float> positionA, Point<float> positionB)
{
    Path path{};
    path.startNewSubPath (positionA);
    path.cubicTo (positionA, getMiddlePoint(positionA, positionB, true), positionB);
    return path;
}

Path Connections::patchCordTypeBCallback (Point<float> positionA, Point<float> positionB)
{
    const Point<float> middlePoint = getMiddlePoint(positionA, positionB);
    
    const float hDistance = abs( positionA.getX() - positionB.getX() ) * 0.2;

    bool order = positionA.x > positionB.x;

    auto cubicHandleA = positionA.translated(order ? -hDistance : hDistance, 0);
    auto cubicHandleB = positionB.translated(order ? hDistance : -hDistance, 0);

    Path path{};
    path.startNewSubPath (positionA);
    path.cubicTo (positionA , cubicHandleA , middlePoint);
    path.cubicTo (middlePoint , cubicHandleB , positionB);
    return path;
}
