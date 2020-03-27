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
phi_Inlet::phi_Inlet() :
inletID(time(NULL))
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
    sendActionMessage("inlet mouseDown " + getCenterAsRectangleString());
}

void phi_Inlet::mouseUp(const MouseEvent& e)
{
    triggerAsyncUpdate ();
}

void phi_Inlet::handleAsyncUpdate()
{
    sendActionMessage("mouseUp");
}

void phi_Inlet::mouseDrag(const MouseEvent& e)
{
    DragAndDropContainer::findParentDragContainerFor(this)->
    startDragging ("inlet" + String(inletID)
                   , this
                   , Image (Image::PixelFormat::RGB, 1, 1, true));
    
    sendActionMessage("draggingPatchCord");
}

bool phi_Inlet::isInterestedInDragSource (const SourceDetails& dragSourceDetails)
{
    return dragSourceDetails.description.toString().contains("outlet");
}

void phi_Inlet::itemDropped (const SourceDetails& dragSourceDetails)
{
    sendActionMessage("dropped " + getCenterAsRectangleString());
}

String phi_Inlet::getCenterAsRectangleString ()
{
    return getTopLevelComponent()->getLocalPoint(this, getLocalBounds().getCentre()).toString() + ", 1, 1";
}

String phi_Inlet::getMouseAsRectangleString (const MouseEvent& e)
{
    return e.getEventRelativeTo(getTopLevelComponent())
           .getPosition().toString()
           + ", 1, 1";
}
