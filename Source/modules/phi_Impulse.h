/*
  ==============================================================================

    phi_Impulse.h
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
class phi_Impulse    : public Component,
                               public Slider::Listener
{
public:
    phi_Impulse();
    ~phi_Impulse();

    void paint (Graphics&) override;
    void resized() override;

private:
    
    phi_Dial decaySlider;
    phi_Dial shapeSlider;
    Rectangle<float> waveViewPort;
    void sliderValueChanged (Slider*) override;
    void decaySliderChanged (float value);
    void shapeSliderChanged (float value);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (phi_Impulse)
};