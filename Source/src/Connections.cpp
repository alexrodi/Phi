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
        g.strokePath ( dragPath, strokeType );
    }
    
    if (connections.size())
    {
        g.setColour (Colours::grey);
        g.fillPath (allConnectionsPath);
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
    for (auto& connection : connections)
    {
        auto path = getConnectionPath (getPlugCenterPositionFromId(Plug::Mode::Inlet, connection.destination)
                                              , getPlugCenterPositionFromId(Plug::Mode::Outlet, connection.source));
        
        juce::Path strokePath;
        
        strokeType.createStrokedPath(strokePath, path);
        
        connection.path.swapWithPath(strokePath);
        
        allConnectionsPath.addPath (connection.path);
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

void Connections::onPlugEvent (const Plug::Event& event)
{
    // Here we receive events from inlets and outlets
    
    typedef Plug::Event::Type EventType;
    
    if (auto object = event.as<Plug::MouseDown>(EventType::MouseDown)) {
        dragPathAnchor = getPlugCenterPositionFromId(object->mode, object->plugID);
    } else if (auto object = event.as<Plug::MouseUp>(EventType::MouseUp)) {
        // When plugs get clicked they might bring the module to the front
        // this assures "Connections" always stays in front
        toFront(false);
        dragPath.clear();
        repaint();
    } else if (auto object = event.as<Plug::Drag>(EventType::Drag)) {
        dragPath = getConnectionPath (dragPathAnchor, getMouseXYRelative().toFloat());
        repaint();
    } else if (auto object = event.as<Plug::Connect>(EventType::Connect)) {
        createConnection (Connection(object->source, object->destination));
        updateAllConnectionPaths();
    } else if (auto object = event.as<Plug::Disconnect>(EventType::Disconnect)) {
        // Not implemented
    }
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
    
    const float hDistance = fabsf( positionA.getX() - positionB.getX() ) * 0.2f;

    bool order = positionA.x > positionB.x;

    auto cubicHandleA = positionA.translated(order ? -hDistance : hDistance, 0);
    auto cubicHandleB = positionB.translated(order ? hDistance : -hDistance, 0);

    Path path{};
    path.startNewSubPath (positionA);
    path.cubicTo (positionA , cubicHandleA , middlePoint);
    path.cubicTo (middlePoint , cubicHandleB , positionB);
    return path;
}

void Connections::onMouseDown(const MouseEvent& e)
{
    for (auto& connection : connections)
    {
        // WORKING HERE
        if (connection.path.contains(e.position, 2.0f)) {
            
            break;
        }
    }
}
