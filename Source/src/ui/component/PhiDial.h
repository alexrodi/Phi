/*
  ==============================================================================

    PhiDial.h
    Created: 9 Feb 2020 11:02:38pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class PhiDial    : public juce::Slider,
public juce::Slider::Listener
{
public:
    PhiDial();
    explicit PhiDial(juce::RangedAudioParameter& parameter);
    ~PhiDial();
    
    void paint (juce::Graphics& g) override;
    void resized () override;
    
    void lookAndFeelChanged() override;
    
private:
    juce::Path groove, dial, pointerPath;
    juce::AffineTransform pointerRotation;
    
    juce::Rectangle<float> box;
    juce::Rectangle<int> nameBounds;
    juce::Colour colour, grooveColour, nameColour;
    
    std::unique_ptr<juce::SliderParameterAttachment> attachment;
    
    const int padding = 10;
    const float thickness = 3.0f;
    const float startAngle = getRotaryParameters().startAngleRadians;
    const float endAngle = getRotaryParameters().endAngleRadians;
    const float angleRange = endAngle - startAngle;
    const juce::PathStrokeType stroke {thickness, juce::PathStrokeType::JointStyle::mitered, juce::PathStrokeType::EndCapStyle::rounded};
    
    float size, radius;
    float angle = startAngle;
    
    bool shouldDrawText = false;
    
    void updateDial();
    
    void sliderValueChanged (juce::Slider*) override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PhiDial)
};
