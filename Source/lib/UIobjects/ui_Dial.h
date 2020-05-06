/*
  ==============================================================================

    phi_Dial.h
    Created: 9 Feb 2020 11:02:38pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

///@cond
#include <JuceHeader.h>
///@endcond

//==============================================================================
/*
*/
class ui_Dial    : public Slider,
                   public Slider::Listener
{
public:
    ui_Dial(std::string, float, float, Slider::Listener*, double = 1, std::string = "", int = 2);
    ~ui_Dial();
    
    void paint (Graphics& g) override;
    void resized () override;
    
    void lookAndFeelChanged() override;
    
private:
    
    NormalisableRange<double> valueRange;
    
    Path groove;
    Path dial;
    Path pointerPath;
    AffineTransform pointerRotation;
    
    Rectangle<float> box;
    Rectangle<int> nameBounds;
    Colour colour;
    Colour grooveColour;
    Colour nameColour;
    
    const int padding = 10;
    const int thickness = 3;
    const float startAngle = getRotaryParameters().startAngleRadians;
    const float endAngle = getRotaryParameters().endAngleRadians;
    const float angleRange = endAngle - startAngle;
    
    float size;
    float radius;
    float angle = startAngle;
    
    bool shouldDrawText = false;
    
    void updateDial();
    
    void sliderValueChanged (Slider*) override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ui_Dial)
};
