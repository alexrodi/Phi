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

Connections::Connections() :
updateConnectionPath{patchCordTypeBCallback}
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

Path Connections::getConnectionPath (Connection* connection)
{
    //this is here for future compatibility with changing connections with the mouse
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

Connections::IOid Connections::registerInlet (uint32 nodeId, phi_Inlet* inlet)
{
    return idStore.storeInlet(nodeId, inlet);
}

Connections::IOid Connections::registerOutlet (uint32 nodeId, phi_Outlet* outlet)
{
    return idStore.storeOutlet(nodeId, outlet);
}

Point<float> Connections::getInletCenterPositionFromId (const IOid inletId)
{
    Component* inlet = idStore.inlets[inletId.first][inletId.second];
    
    return getLocalPoint(inlet, inlet->getLocalBounds().getCentre().toFloat()) ;
}

Point<float> Connections::getOutletCenterPositionFromId (const IOid outletId)
{
    Component* outlet = idStore.outlets[outletId.first][outletId.second];
    
    return getLocalPoint(outlet, outlet->getLocalBounds().getCentre().toFloat()) ;
}

void Connections::updateAllConnectionPaths ()
{
    allConnectionsPath.clear();
    for (Connection* connection : connections)
    {
        updateConnectionPath( connection->path
                              , getInletCenterPositionFromId(connection->inletId)
                              , getOutletCenterPositionFromId(connection->outletId));
        
        allConnectionsPath.addPath(getConnectionPath(connection));
    }
    repaint();
}

Connections::IOid Connections::stringToIOid (const String& stringToParse)
{
    return IOid(stringToParse.upToFirstOccurrenceOf(">", false, false).toUTF8().getIntValue32()
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
        String receivedIdString = message.fromFirstOccurrenceOf("#", false, false);
        
        if (message.containsWholeWord ("inlet"))
        {
            dragPathAnchor = getInletCenterPositionFromId(stringToIOid(receivedIdString));
        }
        else if (message.containsWholeWord ("outlet"))
        {
            dragPathAnchor = getOutletCenterPositionFromId(stringToIOid(receivedIdString));
        }
    }
    else if (message.containsWholeWord ("dragging"))
    {
        updateConnectionPath (dragPath, dragPathAnchor, getMouseXYRelative().toFloat());
        repaint();
    }
    else if (message.containsWholeWord ("mouseUp"))
    {
        // When inlets get clicked they might bring the module to the front
        // this assures "Connections" always stays in front
        toFront(false);
        dragPath.clear();
        repaint();
    }
    else if (message.containsWholeWord ("connect"))
    {
        const String inletIdString = message.fromFirstOccurrenceOf("connect ", false, false).upToFirstOccurrenceOf("&", false, false);
        const String outletIdString = message.fromFirstOccurrenceOf("&", false, false);
        
        createConnection(stringToIOid(inletIdString), stringToIOid(outletIdString));
        updateAllConnectionPaths();
    }
}

const bool Connections::hasConnectionWithIds(const IOid inletId, const IOid outletId)
{
    for (Connection* connection : connections)
    {
        if (connection->inletId == inletId && connection->outletId == outletId)
            return true;
    }
    return false;
}

void Connections::createConnection(const IOid inletId, const IOid outletId)
{
    if (! hasConnectionWithIds(inletId, outletId))
    {
        connections.add( new Connection( updateConnectionPath
                         , inletId
                         , outletId
                         , getInletCenterPositionFromId(inletId)
                         , getOutletCenterPositionFromId(outletId)));
    }
    sendChangeMessage(); // notify new connections
}

void Connections::removeModule(uint32 moduleId)
{
    int i = 0;
    while (i < connections.size())
    {
        if (connections[i]->inletId.first == moduleId || connections[i]->outletId.first == moduleId)
        {
            connections.remove(i);
        }
        else i++;
    }
    idStore.removeModule(moduleId);
    updateAllConnectionPaths();
}

Array<std::pair<Connections::IOid, Connections::IOid>> Connections::getAllConnectionIdPairs()
{
    Array<std::pair<IOid, IOid>> allConnections;
    for (Connection* connection : connections)
    {
        allConnections.add(std::pair<IOid, IOid>(connection->outletId, connection->inletId));
    }
    return allConnections;
}

void Connections::togglePatchCordType()
{
    patchCordTypeToggle = !patchCordTypeToggle;
    updateConnectionPath = patchCordTypeToggle ? patchCordTypeACallback : patchCordTypeBCallback;
    updateAllConnectionPaths();
}

Point<float> getMiddlePoint (Point<float> point1, Point<float> point2, bool applyWeight = false)
{
    const float distance = point1.getDistanceFrom(point2);
    const Point<float> middlePoint = point1.getPointOnCircumference(distance * 0.5, point1.getAngleToPoint(point2));
    
    if (applyWeight)
    {
        return middlePoint.translated(0.0f, distance * CORD_WEIGHT);
    }
    
    return middlePoint;
}

void Connections::patchCordTypeACallback (Path& path, Point<float> positionA, Point<float> positionB)
{
    path.clear();
    path.startNewSubPath (positionA);
    path.cubicTo (positionA, getMiddlePoint(positionA, positionB, true), positionB);
}

void Connections::patchCordTypeBCallback (Path& path, Point<float> positionA, Point<float> positionB)
{
    const Point<float> middlePoint = getMiddlePoint(positionA, positionB);
    
    const float hDistance = abs( positionA.getX() - positionB.getX() ) * 0.2;

    bool order = positionA.x > positionB.x;

    Point<float> cubicHandleA = positionA.translated(order ? -hDistance : hDistance, 0);
    Point<float> cubicHandleB = positionB.translated(order ? hDistance : -hDistance, 0);

    path.clear();
    path.startNewSubPath (positionA);
    path.cubicTo (positionA , cubicHandleA , middlePoint);
    path.cubicTo (middlePoint , cubicHandleB , positionB);
}
