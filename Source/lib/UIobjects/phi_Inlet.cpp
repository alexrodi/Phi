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
    sendActionMessage("inlet mouseDown"
                      + e.withNewPosition(getLocalBounds().getCentre())
                      .getEventRelativeTo(getParentComponent()->getParentComponent()->getParentComponent())
                      .getPosition().toString()
                      + ", 1, 1");
}

void phi_Inlet::mouseUp(const MouseEvent& e)
{
    // this only notifies of a mouse-up event,
    // it says nothing about where the connection was released
    // or even IF it was released over a valid outlet
    sendActionMessage("inlet mouseUp");
}

void phi_Inlet::mouseDrag(const MouseEvent& e)
{
    sendActionMessage("inlet mouseDrag "
                      + e.getEventRelativeTo(getParentComponent()->getParentComponent()->getParentComponent())
                      .getPosition().toString()
                      + ", 1, 1");
}

