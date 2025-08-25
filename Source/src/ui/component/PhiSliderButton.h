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
    explicit PhiSliderButton(const juce::String& leftText = "", const juce::String& rightText = "");
    ~PhiSliderButton();

    void paintButton (juce::Graphics&, bool, bool) override;
    void resized() override;
    
    void colourChanged() override;

private:
    const int knobSize = 12;
    const float sliderWidth = (float)knobSize * 2.5f;
    bool lastToggle = false;
    
    struct Knob : juce::Component {
        Knob() {
            setPaintingIsUnclipped(true);
            setInterceptsMouseClicks(false, false);
        }
        
        void paint(juce::Graphics& g) override {
            g.setColour(findColour(juce::Slider::thumbColourId));
            g.fillEllipse(getLocalBounds().toFloat());
        }
    } knob;
    
    juce::String leftText, rightText;
    juce::Rectangle<float> sliderBounds, leftTextBounds, rightTextBounds;
    
    juce::Colour textColour;
    
    juce::Rectangle<int> getKnobBounds();
    
    void buttonStateChanged() override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PhiSliderButton)
};
