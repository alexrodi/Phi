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
updateConnectionPath{patchCordTypeACallback}
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
                      , PathStrokeType ( 4.0f
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

Point<float> Connections::getInletCenterPositionFromString (const String& inletId)
{
    Component* inlet = idStore.inlets[inletId.toUTF8().getIntValue32()];
    
    return getLocalPoint(inlet, inlet->getLocalBounds().getCentre().toFloat()) ;
}

Point<float> Connections::getOutletCenterPositionFromString (const String& outletId)
{
    Component* outlet = idStore.outlets[outletId.toUTF8().getIntValue32()];
    
    return getLocalPoint(outlet, outlet->getLocalBounds().getCentre().toFloat()) ;
}

void Connections::updateAllConnectionPaths ()
{
    allConnectionsPath.clear();
    for (Connection* connection : connections)
    {
        updateConnectionPath( connection->path
                              , getInletCenterPositionFromString(connection->inletId)
                              , getOutletCenterPositionFromString(connection->outletId));
        
        allConnectionsPath.addPath(getConnectionPath(connection));
    }
    repaint();
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
        String receivedId = message.fromFirstOccurrenceOf("#", false, false);
        
        if (message.containsWholeWord ("inlet"))
        {
            dragPathAnchor = getInletCenterPositionFromString(receivedId);
        }
        else if (message.containsWholeWord ("outlet"))
        {
            dragPathAnchor = getOutletCenterPositionFromString(receivedId);
        }
    }
    else if (message.containsWholeWord ("dragging"))
    {
        updateConnectionPath (dragPath, dragPathAnchor, getMouseXYRelative().toFloat());
        repaint();
    }
    else if (message.containsWholeWord ("mouseUp"))
    {
        dragPath.clear();
        repaint();
    }
    else if (message.containsWholeWord ("connect"))
    {
        const String inletId = message.fromFirstOccurrenceOf("connect ", false, false).upToFirstOccurrenceOf("&", false, false);
        const String outletId = message.fromFirstOccurrenceOf("&", false, false);
        
        createConnection(outletId, inletId);
        updateAllConnectionPaths();
    }
}

void Connections::createConnection(const String& outletId, const String& inletId)
{
    connections.add( new Connection( updateConnectionPath
                                    , inletId
                                    , outletId
                                    , getInletCenterPositionFromString(inletId)
                                    , getOutletCenterPositionFromString(outletId)));
    
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
