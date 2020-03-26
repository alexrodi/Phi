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

String phi_Outlet::getCenterAsRectangleString (const MouseEvent& e)
{
    return e.withNewPosition(getLocalBounds().getCentre())
           .getEventRelativeTo(getParentComponent()->getParentComponent()->getParentComponent())
           .getPosition().toString()
           + ", 1, 1";
}

String phi_Outlet::getMouseAsRectangleString (const MouseEvent& e)
{
    return e.getEventRelativeTo(getParentComponent()->getParentComponent()->getParentComponent())
           .getPosition().toString()
           + ", 1, 1";
}

void phi_Outlet::mouseDown(const MouseEvent& e)
{
    sendActionMessage("outlet mouseDown " + getCenterAsRectangleString(e));
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
    sendActionMessage("outlet mouseDrag " + getMouseAsRectangleString(e));
}

void phi_Outlet::mouseEnter(const MouseEvent& e)
{
    sendActionMessage("outlet mouseEnter " + getCenterAsRectangleString(e));

}

void phi_Outlet::mouseExit(const MouseEvent& e)
{
    sendActionMessage("outlet mouseExit " + getCenterAsRectangleString(e));

}
