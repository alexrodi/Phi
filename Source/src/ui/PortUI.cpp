/*
  ==============================================================================

    PortUI.cpp
    Created: 16 Apr 2020 11:08:52pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#include "PortUI.h"

//==============================================================================

PortUI::PortUI(PortType type, const juce::String& name) :
HoverPopupClient(this),
type(type)
{
    setName(name);
    
    colors = type == PortType::Inlet
    ? PortColors{juce::Colours::grey, juce::Colours::darkgrey}
    : PortColors{juce::Colours::darkgrey, juce::Colours::grey};
    
    setPaintingIsUnclipped(true);
}

void PortUI::paint (juce::Graphics& g)
{
    if (drawText) {
        g.setColour(juce::Colours::grey);
        g.drawText(getName(), nameBounds, juce::Justification::centredTop);
    }
    
    g.setColour(colors.inner);
    g.fillEllipse(portBounds);
    g.setColour(colors.outer);
    g.drawEllipse(portBounds, portStroke);
}

void PortUI::resized()
{
    const int textHeight = 18;
    const int portHeight = portSize + portStroke;
    
    bool canFitText = getHeight() > portHeight + textHeight + 12;
    drawText = shouldShowLabel == ShowPortLabels::On && canFitText;
    setHoverPopupEnabled(!drawText);
    
    auto bounds = getLocalBounds().withSizeKeepingCentre(getWidth(), portHeight + (drawText ? textHeight : 0));
    
    portBounds = bounds.removeFromBottom(portHeight).toFloat().withSizeKeepingCentre(portSize, portSize);
    nameBounds = bounds;
}

juce::Point<float> PortUI::hoverPopupPosition() {
    return getLocalBounds().toFloat().getCentre().translated(0, -portSize - 10);
}

juce::String PortUI::getPopupText() { return getName(); }
