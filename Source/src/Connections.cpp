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

Connections::PlugID Connections::registerPlug (PlugMode plugMode, uint32 nodeId, Component* inlet)
{
    return idStore.storePlug(plugMode, nodeId, inlet);
}

Point<float> Connections::getPlugCenterPositionFromId (PlugMode plugMode, const PlugID inletId)
{
    auto plug = (plugMode == Inlet ? idStore.inlets : idStore.outlets)[inletId.first][inletId.second];
    
    return getLocalPoint(plug, plug->getLocalBounds().getCentre().toFloat()) ;
}

void Connections::updateAllConnectionPaths ()
{
    allConnectionsPath.clear();
    for (Connection connection : connections)
    {
        allConnectionsPath.addPath (getConnectionPath (getPlugCenterPositionFromId(Inlet, connection.second)
                                                       , getPlugCenterPositionFromId(Outlet, connection.first)));
    }
    repaint();
}

Connections::PlugID Connections::stringToPlugID (const String& stringToParse)
{
    return PlugID(stringToParse.upToFirstOccurrenceOf(">", false, false).toUTF8().getIntValue32()
                , stringToParse.fromFirstOccurrenceOf(">", false, false).toUTF8().getIntValue32());
}

void Connections::actionListenerCallback (const String& message)
{
    // Here we receive events from inlets, outlets and moduleBoxes
    // std::cout << message << std::endl;
    
    if (message.containsWholeWord ("moduleChanged"))
    {
        updateAllConnectionPaths();
    }
    else if (message.containsWholeWord ("mouseDown"))
    {
        PlugID plugId = stringToPlugID(message.fromFirstOccurrenceOf("#", false, false));
        
        if (message.containsWholeWord ("inlet"))
        {
            dragPathAnchor = getPlugCenterPositionFromId(Inlet, plugId);
        }
        else if (message.containsWholeWord ("outlet"))
        {
            dragPathAnchor = getPlugCenterPositionFromId(Outlet, plugId);
        }
    }
    else if (message.containsWholeWord ("dragging"))
    {
        dragPath = getConnectionPath (dragPathAnchor, getMouseXYRelative().toFloat());
        repaint();
    }
    else if (message.containsWholeWord ("mouseUp"))
    {
        // When plugs get clicked they might bring the module to the front
        // this assures "Connections" always stays in front
        toFront(false);
        dragPath.clear();
        repaint();
    }
    else if (message.containsWholeWord ("connect"))
    {
        const String inletIdString = message.fromFirstOccurrenceOf("connect ", false, false).upToFirstOccurrenceOf("&", false, false);
        const String outletIdString = message.fromFirstOccurrenceOf("&", false, false);
        
        createConnection (stringToPlugID(inletIdString), stringToPlugID(outletIdString));
        updateAllConnectionPaths();
    }
}

void Connections::createConnection(const PlugID inletId, const PlugID outletId)
{
    connections.addIfNotAlreadyThere( Connection(outletId, inletId) );
    sendChangeMessage(); // notify new connections
}

void Connections::removeModule(uint32 moduleId)
{
    int i = 0;
    while (i < connections.size())
    {
        if (connections[i].second.first == moduleId || connections[i].first.first == moduleId)
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
