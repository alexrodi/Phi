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
    if (drawName && canFitText) {
        g.setColour(juce::Colours::grey);
        g.drawText(getName(), nameBounds, nameJustification);
        setHoverPopupEnabled(false);
    } else {
        setHoverPopupEnabled(true); // Display the tooltip if not showing text
    }
    
    g.setColour(colors.inner);
    g.fillEllipse(bounds);
    g.setColour(colors.outer);
    g.drawEllipse(bounds, STROKE_WIDTH);
}

void PortUI::resized()
{
    nameJustification = juce::Justification::Flags::centredBottom;
    nameBounds = getLocalBounds().withTrimmedBottom(getHeight() * 0.5 + 12);
    canFitText = nameBounds.getHeight() > 11;
    
    bounds = getLocalBounds().toFloat().withSizeKeepingCentre(SIZE, SIZE);
}

juce::Point<float> PortUI::hoverPopupPosition() {
    return getLocalBounds().toFloat().getCentre().translated(0, -SIZE - 10);
}

juce::String PortUI::getPopupText() { return getName(); }
