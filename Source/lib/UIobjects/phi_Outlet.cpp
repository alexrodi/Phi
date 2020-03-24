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
    sendActionMessage("outlet mouseDown " + e.withNewPosition(getLocalBounds().getCentre()).getEventRelativeTo(getParentComponent()->getParentComponent()->getParentComponent()).getPosition().toString() + ", 1, 1");
}

void phi_Outlet::mouseUp(const MouseEvent& e)
{
    // this only notifies of a mouse-up event,
    // it says nothing about where the connection was released
    // or even IF it was released over a valid inlet
    sendActionMessage("outlet mouseUp");
}

void phi_Outlet::mouseDrag(const MouseEvent& e)
{
    sendActionMessage("outlet mouseDrag " + e.getEventRelativeTo(getParentComponent()->getParentComponent()->getParentComponent()).getPosition().toString() + ", 1, 1");
}
