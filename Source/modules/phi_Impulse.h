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
#include "../objects/phi_Inlet.h"
#include "../objects/phi_Outlet.h"

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
    
    String moduleName;

private:
    
    // This is how we keep our inlets & outlets,
    // they belong to the module itself (not the box)
    OwnedArray<phi_Inlet> inlets;
    OwnedArray<phi_Outlet> outlets;
    
    //Dials
    phi_Dial decaySlider;
    phi_Dial shapeSlider;
    
    //Waveform
    Rectangle<float> waveViewPort;
    
    //Listeners
    void sliderValueChanged (Slider*) override;
    void decaySliderChanged (float value);
    void shapeSliderChanged (float value);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (phi_Impulse)
};
