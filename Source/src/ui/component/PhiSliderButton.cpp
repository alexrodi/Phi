/*
  ==============================================================================

    PhiSliderButton.cpp
    Created: 15 Apr 2020 12:20:11am
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#include "PhiSliderButton.h"

//==============================================================================
PhiSliderButton::PhiSliderButton(const juce::String& labelText, LabelPosition labelPositionToUse) :
Button{labelText},
labelPosition{labelPositionToUse},
labelTextJustification{juce::Justification::centredTop}
{
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
    if (shouldDraw)
    {
        g.setColour(textColour);
        g.drawFittedText(getName(), labelBounds, labelTextJustification, 1);
        
        g.setColour(juce::Colour::greyLevel(0.18));
        g.fillRoundedRectangle(sliderBounds.reduced(1), SLIDER_CORNER);
    }
}

void PhiSliderButton::resized()
{
    juce::Path knobpath{};
    
    if (getWidth() > SLIDER_WIDTH && getWidth() > SLIDER_SIZE)
    {
        shouldDraw = true;
        
        juce::Rectangle<float> bounds{getLocalBounds().toFloat()};
        
        switch (labelPosition) {
            case LabelPosition::Above:
                sliderBounds = bounds.removeFromBottom(SLIDER_SIZE);
                labelTextJustification = juce::Justification::centredTop;
                break;
            case LabelPosition::Below:
                sliderBounds = bounds.removeFromTop(SLIDER_SIZE);
                labelTextJustification = juce::Justification::centredBottom;
                break;
            case LabelPosition::Left:
                sliderBounds = bounds.removeFromRight(SLIDER_WIDTH);
                labelTextJustification = juce::Justification::centredLeft;
                break;
            case LabelPosition::Right:
                sliderBounds = bounds.removeFromLeft(SLIDER_WIDTH);
                labelTextJustification = juce::Justification::centredRight;
                break;
        }
        
        sliderBounds = sliderBounds.withSizeKeepingCentre(SLIDER_WIDTH, SLIDER_SIZE);
        
        labelBounds = bounds.toNearestInt();
        
        juce::Point<int> ellipsePoint (getKnobBounds().getTopLeft());
        knobpath.addEllipse(ellipsePoint.x, ellipsePoint.y, SLIDER_SIZE, SLIDER_SIZE);
    }
    else shouldDraw = false;
    
    knob.setPath(knobpath);
}

juce::Rectangle<int> PhiSliderButton::getKnobBounds()
{
    return sliderBounds
           .toNearestInt()
           .withWidth(SLIDER_SIZE)
           .translated( getToggleState() ? sliderBounds.getWidth()-SLIDER_SIZE : 0, 0);
}

void PhiSliderButton::clicked()
{
    juce::Desktop::getInstance().getAnimator().animateComponent(&knob, getKnobBounds(), 1.0f, 300, false, 0.5f, 0.5f);
}

void PhiSliderButton::lookAndFeelChanged()
{
    knob.setFill(findColour(juce::Slider::thumbColourId));
    textColour = findColour(juce::TextButton::textColourOnId);
}
