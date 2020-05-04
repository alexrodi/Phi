/*
  ==============================================================================

    Gain.h
    Created: 29 Mar 2020 3:14:32pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

///@cond
#include <JuceHeader.h>
///@endcond
#include "GainProcessor.h"
#include "../../ModuleUI.h"
#include "../../UIobjects/ui_Dial.h"

class GainUI    : public ModuleUI,
                  public Slider::Listener
{
public:
    GainUI(GainProcessor&);
    ~GainUI();

    void paint (Graphics&) override;
    void wasResized(Rectangle<int> moduleBounds) override;

private:
    ui_Dial gainDial;

    void sliderValueChanged (Slider* slider) override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GainUI)
};


