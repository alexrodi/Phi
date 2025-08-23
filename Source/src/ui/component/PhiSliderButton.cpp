/*
  ==============================================================================

    PhiSliderButton.cpp
    Created: 15 Apr 2020 12:20:11am
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#include "PhiSliderButton.h"

//==============================================================================
PhiSliderButton::PhiSliderButton(const juce::String& leftText, const juce::String& rightText) :
juce::Button(""),
leftText(leftText),
rightText(rightText)
{
    juce::Path knobPath;
    knobPath.addEllipse(0, 0, knobSize, knobSize);
    
    knob.setPath(knobPath);
    addAndMakeVisible(knob);
    knob.setPaintingIsUnclipped(true);
    
    setClickingTogglesState (true);
    setPaintingIsUnclipped(true);
}

PhiSliderButton::~PhiSliderButton()
{
}

void PhiSliderButton::paintButton (juce::Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
    if (getWidth() > sliderWidth && getWidth() > knobSize)
    {
        g.setColour(textColour);
        g.drawText(leftText, leftTextBounds, juce::Justification::centred, 1);
        g.drawText(rightText, rightTextBounds, juce::Justification::centred, 1);
        
        g.setColour(juce::Colour::greyLevel(0.18));
        g.fillRoundedRectangle(sliderBounds, sliderBounds.getHeight() * 0.5f);
    }
}

void PhiSliderButton::resized()
{
    auto bounds = getLocalBounds().toFloat();
    
    int numLabels = (leftText.isNotEmpty() ? 1 : 0) + (rightText.isNotEmpty() ? 1 : 0);
    float labelWidth = (bounds.getWidth() - sliderWidth) / (float)numLabels;
    
    if (leftText.isNotEmpty())
        leftTextBounds = bounds.removeFromLeft(labelWidth);
    
    if (rightText.isNotEmpty())
        rightTextBounds = bounds.removeFromRight(labelWidth);
    
    sliderBounds = bounds.withSizeKeepingCentre(sliderWidth, knobSize).reduced(1);
    
    buttonStateChanged();
}

void PhiSliderButton::buttonStateChanged()
{
    auto knobBounds = sliderBounds.withSize(knobSize, knobSize)
        .translated(getToggleState() ? sliderBounds.getWidth()-knobSize : 0, -1)
        .toNearestInt();
    
    juce::Desktop::getInstance().getAnimator().animateComponent(&knob, knobBounds, 1.0f, 100, false, 0.5f, 0.5f);
}

void PhiSliderButton::lookAndFeelChanged()
{
    knob.setFill(findColour(juce::Slider::thumbColourId));
    textColour = findColour(juce::TextButton::textColourOnId);
}
