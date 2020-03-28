/*
  ==============================================================================

    phi_Inlet.cpp
    Created: 18 Feb 2020 11:19:12pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#include <JuceHeader.h>
#include "phi_Inlet.h"

//==============================================================================
phi_Inlet::phi_Inlet()
{
    setPaintingIsUnclipped(true);
}

phi_Inlet::~phi_Inlet()
{
}

void phi_Inlet::paint (Graphics& g)
{
    g.setColour(Colours::grey);
    g.fillEllipse(viewport);
    g.setColour(Colours::darkgrey);
    g.drawEllipse(viewport, 3);
}

void phi_Inlet::resized()
{
    viewport = getLocalBounds().withSizeKeepingCentre(12, 12).toFloat();
}

void phi_Inlet::mouseDown(const MouseEvent& e)
{
    sendActionMessage("mouseDown " + getCenterAsRectangleString(this));
}

void phi_Inlet::mouseUp(const MouseEvent& e)
{
    sendActionMessage("mouseUp");
}

void phi_Inlet::mouseDrag(const MouseEvent& e)
{
    DragAndDropContainer::findParentDragContainerFor(this)->
    startDragging ("inlet" + String(inletID)
                   , this
                   , Image (Image::PixelFormat::RGB, 1, 1, true));
    
    sendActionMessage("dragging");
}

bool phi_Inlet::isInterestedInDragSource (const SourceDetails& dragSourceDetails)
{
    return dragSourceDetails.description.toString().contains("outlet");
}

void phi_Inlet::itemDropped (const SourceDetails& dragSourceDetails)
{
    sendActionMessage("connect "
                      + String(inletID) + "&"
                      + dragSourceDetails.description.toString().fromFirstOccurrenceOf("outlet", false, false));
}

String phi_Inlet::getCenterAsRectangleString(Component* component)
{
    return getTopLevelComponent()->getLocalPoint(component, getLocalBounds().getCentre()).toString() + ", 1, 1";
}

String phi_Inlet::getMouseAsRectangleString (const MouseEvent& e)
{
    return e.getEventRelativeTo(getTopLevelComponent())
           .getPosition().toString()
           + ", 1, 1";
}
