/*
  ==============================================================================

    PortUI.cpp
    Created: 16 Apr 2020 11:08:52pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#include "PortUI.h"
#include "PhiColours.h"

//==============================================================================

PortUI::PortUI(PortType type, const juce::String& name) :
HoverPopupClient(this),
type(type)
{
    setName(name);
    setPaintingIsUnclipped(true);
}

void PortUI::paint (juce::Graphics& g)
{
    if (drawText) {
        g.setColour(findColour(PhiColourIds::Port::Text));
        g.drawText(getName(), nameBounds, juce::Justification::centredTop);
    }
    
    g.setColour(colors.inner);
    g.fillEllipse(portBounds);
    g.setColour(colors.outer);
    g.drawEllipse(portBounds, portStroke);
}

void PortUI::resized()
{
    canFitText = getHeight() > portHeight + textHeight + 12;
    drawText = shouldShowLabel == ShowPortLabels::On && canFitText;
    
    portBounds = getPortBounds(shouldShowLabel);
    nameBounds = {0, (int)portBounds.getY() - textHeight, getWidth(), textHeight};
    
    setHoverPopupEnabled(!drawText);
}

juce::Rectangle<float> PortUI::getPortBounds(ShowPortLabels showLabel) const {
    bool drawText = showLabel == ShowPortLabels::On && canFitText;
    
    auto bounds = getLocalBounds().withSizeKeepingCentre(getWidth(), portHeight + (drawText ? textHeight : 0));
    
    return bounds.removeFromBottom(portHeight).toFloat().withSizeKeepingCentre(portSize, portSize);
}

juce::Point<float> PortUI::hoverPopupPosition() {
    return getLocalBounds().toFloat().getCentre().translated(0, -portSize - 10);
}

juce::String PortUI::getPopupText() { return getName(); }

void PortUI::colourChanged() {
    if (type == PortType::Inlet) {
        colors = {
            .inner = findColour(PhiColourIds::Port::IntletFill),
            .outer = findColour(PhiColourIds::Port::IntletOutline)
        };
    } else if (type == PortType::Outlet) {
        colors = {
            .inner = findColour(PhiColourIds::Port::OutletFill, true),
            .outer = findColour(PhiColourIds::Port::OutletOutline)
        };
    }
}
