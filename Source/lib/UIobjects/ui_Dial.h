/*
  ==============================================================================

    phi_Dial.h
    Created: 9 Feb 2020 11:02:38pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class ui_Dial    : public Slider,
                   public Slider::Listener
{
public:
    ui_Dial(float, float, std::string, int, Slider::Listener*);
    ~ui_Dial();
    
    void paint (Graphics& g) override;
    void resized () override;
    
    void lookAndFeelChanged() override;
    
private:
    
    Path groove;
    Path dial;
    Path pointerPath;
    AffineTransform pointerRotation;
    
    Rectangle<float> box;
    Colour colour;
    Colour grooveColour;
    
    const int padding = 10;
    const int thickness = 5;
    const float startAngle = getRotaryParameters().startAngleRadians;
    const float endAngle = getRotaryParameters().endAngleRadians;
    const float range = endAngle - startAngle;
    
    float size;
    float radius;
    float angle = startAngle;
    
    void updateDial();
    
    void sliderValueChanged (Slider*) override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ui_Dial)
};
