/*
  ==============================================================================

    Gain.h
    Created: 29 Mar 2020 3:14:32pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../Module.h"
#include "../UIobjects/ui_Dial.h"

class module_Gain    : public Module,
                       public Slider::Listener
{
public:
    module_Gain();
    ~module_Gain();

    void paint (Graphics&) override;
    void resized() override;
    
    const void processAudio(const AudioSourceChannelInfo&);

private:
    
    ui_Dial gainDial;

    void sliderValueChanged (Slider*) override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (module_Gain)
};
