/*
  ==============================================================================

    phi_Impulse.h
    Created: 9 Feb 2020 11:02:38pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../UIobjects/phi_Dial.h"
#include "../UIobjects/phi_Inlet.h"
#include "../UIobjects/phi_Outlet.h"

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
    
    /// This is how we keep our inlets & outlets,
    /// they belong to the module itself (not the box) and they should be public
    OwnedArray<phi_Inlet> inlets;
    OwnedArray<phi_Outlet> outlets;

private:
    
    //Dials
    phi_Dial decayDial;
    phi_Dial shapeDial;
    
    //Waveform
    Rectangle<float> waveViewPort;
    
    //Listeners
    void sliderValueChanged (Slider*) override;
    void decayDialChanged (float value);
    void shapeDialChanged (float value);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (phi_Impulse)
};
