/*
  ==============================================================================

    phi_Outlet.cpp
    Created: 18 Feb 2020 11:19:27pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#include <JuceHeader.h>
#include "phi_Outlet.h"

//==============================================================================
phi_Outlet::phi_Outlet()
{
    setPaintingIsUnclipped(true);
}

phi_Outlet::~phi_Outlet()
{
}

void phi_Outlet::paint (Graphics& g)
{
    g.setColour(Colours::darkgrey);
    g.fillEllipse(viewport);
    g.setColour(Colours::grey);
    g.drawEllipse(viewport, 3);
}

void phi_Outlet::resized()
{
    viewport = getLocalBounds().withSizeKeepingCentre(12, 12).toFloat();
}

void phi_Outlet::mouseDown(const MouseEvent& e)
{
    sendActionMessage("mouseDown " + getCenterAsRectangleString(this));
}

void phi_Outlet::mouseUp(const MouseEvent& e)
{
    sendActionMessage("mouseUp");
}

void phi_Outlet::mouseDrag(const MouseEvent& e)
{
    DragAndDropContainer::findParentDragContainerFor(this)->
    startDragging ("outlet" + String(outletID)
                   , this
                   , Image (Image::PixelFormat::RGB, 1, 1, true));
    
    sendActionMessage("dragging");
}

bool phi_Outlet::isInterestedInDragSource (const SourceDetails& dragSourceDetails)
{
    return dragSourceDetails.description.toString().contains("inlet");
}

void phi_Outlet::itemDropped (const SourceDetails& dragSourceDetails)
{
    sendActionMessage("connect "
                      + dragSourceDetails.description.toString().fromFirstOccurrenceOf("inlet", false, false)
                      + "&"
                      + String(outletID));
}

String phi_Outlet::getCenterAsRectangleString(Component* component)
{
    return getTopLevelComponent()->getLocalPoint(component, getLocalBounds().getCentre()).toString() + ", 1, 1";
}

String phi_Outlet::getMouseAsRectangleString (const MouseEvent& e)
{
    return e.getEventRelativeTo(getTopLevelComponent())
           .getPosition().toString()
           + ", 1, 1";
}
