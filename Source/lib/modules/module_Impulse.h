/*
  ==============================================================================

    Impulse.h
    Created: 9 Feb 2020 11:02:38pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../Module.h"
#include "../UIobjects/ui_Dial.h"

//==============================================================================
/*
*/
class module_Impulse    : public Module,
                   public Slider::Listener
{
public:
    module_Impulse();
    ~module_Impulse();

    void paint (Graphics&) override;
    void resized() override;

private:
    
    //Dials
    ui_Dial decayDial;
    ui_Dial shapeDial;
    
    //Waveform
    Rectangle<float> waveViewPort;
    
    //Listeners
    void sliderValueChanged (Slider*) override;
    void decayDialChanged (float value);
    void shapeDialChanged (float value);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (module_Impulse)
};
