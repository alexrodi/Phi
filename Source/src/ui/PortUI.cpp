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
    if (shouldDrawText && canFitText) {
        g.setColour(juce::Colours::grey);
        g.drawText(getName(), nameBounds, juce::Justification::centredBottom);
    }
    
    g.setColour(colors.inner);
    g.fillEllipse(bounds);
    g.setColour(colors.outer);
    g.drawEllipse(bounds, STROKE_WIDTH);
}

void PortUI::resized()
{
    nameBounds = getLocalBounds().withTrimmedBottom(getHeight() * 0.5 + 12);
    bounds = getLocalBounds().toFloat().withSizeKeepingCentre(SIZE, SIZE);
    
    canFitText = nameBounds.getHeight() > 11;
    setHoverPopupEnabled(!(shouldDrawText && canFitText));
}

juce::Point<float> PortUI::hoverPopupPosition() {
    return getLocalBounds().toFloat().getCentre().translated(0, -SIZE - 10);
}

juce::String PortUI::getPopupText() { return getName(); }
