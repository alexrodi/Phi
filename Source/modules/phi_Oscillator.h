/*
  ==============================================================================

    phi_Oscillator.h
    Created: 9 Feb 2020 11:02:38pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../objects/phi_Dial.h"

//==============================================================================
/*
*/
class phi_Oscillator    : public Component,
                               public Slider::Listener
{
public:
    phi_Oscillator();
    ~phi_Oscillator();

    void paint (Graphics&) override;
    void resized() override;

private:
    
    phi_Dial frequencySlider;
    phi_Dial shapeSlider;
    Rectangle<float> waveViewPort;
    void sliderValueChanged (Slider*) override;
    void frequencySliderChanged (float value);
    void shapeSliderChanged (float value);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (phi_Oscillator)
};
