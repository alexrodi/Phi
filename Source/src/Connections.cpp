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

    for (auto& connection : connections)
    {
        if (selectedConnections.isSelected(connection)) {
            g.setColour (Colours::grey.brighter());
            g.strokePath (connection->path, PathStrokeType(2.0f));
        }
        else {
            g.setColour (Colours::grey);
            g.fillPath (connection->path);
        }
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

void Connections::updateConnectionPath (Connection& connection)
{
    auto path = getConnectionPath (getPlugCenterPositionFromId(Plug::Mode::Inlet, connection.destination)
                                          , getPlugCenterPositionFromId(Plug::Mode::Outlet, connection.source));
    
    Path strokePath;
    
    strokeType.createStrokedPath(strokePath, path);
    
    connection.path.swapWithPath(strokePath);
}

void Connections::updateAllConnectionPaths ()
{
    for (auto& connection : connections)
        updateConnectionPath(*connection);
 
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
        createConnection ({object->source, object->destination});
    }
}

void Connections::deselectAll()
{
    selectedConnections.deselectAll();
    repaint();
}

bool Connections::containsConnectionWith (std::pair<PlugID,PlugID>& sourceDestination)
{
    auto* e = connections.begin();

    for (; e != connections.end(); ++e)
        if (sourceDestination.first == (*e)->source && sourceDestination.second == (*e)->destination)
            return true;

    return false;
}

void Connections::createConnection(std::pair<PlugID,PlugID> sourceDestination)
{
    if (!containsConnectionWith(sourceDestination)) {
        auto newConnection = connections.add(std::make_unique<Connection>(sourceDestination));
        
        sendChangeMessage(); // trigger connections refresh
        
        updateConnectionPath(*newConnection);
        repaint();
    }
}

void Connections::removeConnectionsIf(std::function<bool(Connection&)> predicate)
{
    int i = 0;
    while (i < connections.size())
    {
        if (predicate(*connections[i]))
        {
            connections.remove(i);
        }
        else i++;
    }
    selectedConnections.deselectAll();
}

void Connections::removeModule(uint32 moduleId)
{
    removeConnectionsIf( [moduleId] (Connection& connection) {
        return connection.source.moduleID() == moduleId || connection.destination.moduleID() == moduleId;
    });
    idStore.removeModule(moduleId);
    repaint();
}

OwnedArray<Connections::Connection>& Connections::getConnections()
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
        if (connection->path.contains(e.position)) {
            selectedConnections.addToSelectionOnMouseDown(connection, e.mods);
            updateConnectionPath(*connection);
            repaint();
            return;
        }
    }
    selectedConnections.deselectAll();
    repaint();
}

void Connections::deleteAllSelectedConnections()
{
    removeConnectionsIf( [this] (Connection& connection) {
        return selectedConnections.isSelected(&connection);
    });
}

bool Connections::keyPressed (const KeyPress& key)
{
    if (key == KeyPress::backspaceKey)
    {
        deleteAllSelectedConnections();
        sendChangeMessage(); // trigger connections refresh
        repaint();
    }
    return true;
}
