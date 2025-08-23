/*
  ==============================================================================

    PhiSliderButton.h
    Created: 15 Apr 2020 12:20:11am
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
struct PhiSliderButton : juce::Button
{
    enum class LabelPosition
    {
        Above,
        Below,
        Left,
        Right
    };
    
    explicit PhiSliderButton(const juce::String&, LabelPosition = LabelPosition::Above);
    ~PhiSliderButton();

    void paintButton (juce::Graphics&, bool, bool) override;
    void resized() override;
    
    void lookAndFeelChanged() override;

private:
    const float SLIDER_SIZE = 12;
    const float SLIDER_WIDTH = SLIDER_SIZE * 2.5;
    const float SLIDER_CORNER = (SLIDER_SIZE-2) * 0.5;
    
    bool shouldDraw = true;
    
    LabelPosition labelPosition;
    juce::Justification labelTextJustification;
    
    juce::DrawablePath knob;
    
    juce::Rectangle<int> labelBounds;
    juce::Rectangle<float> sliderBounds;
    
    juce::Colour textColour;
    
    juce::Rectangle<int> getKnobBounds();
    
    void clicked() override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PhiSliderButton)
};
