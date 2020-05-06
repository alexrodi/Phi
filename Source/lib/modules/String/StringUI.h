/*
  ==============================================================================

    StringUI.h
    Created: 25 Mar 2020 3:14:32pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

///@cond
#include <JuceHeader.h>
///@endcond
#include "../../ModuleUI.h"
#include "../../UIobjects/ui_Dial.h"
#include "../../UIobjects/ui_SliderButton.h"

class StringUI    : public ModuleUI,
                    public Slider::Listener,
                    public Button::Listener
{
public:
    StringUI(ModuleProcessor&);
    ~StringUI();

    void paint (Graphics&) override;
    void wasResized(Rectangle<int> moduleBounds) override;

private:
    ui_Dial frequencyDial;
    ui_Dial positionDial;
    ui_Dial dampDial;
    ui_Dial decayDial;
    ui_SliderButton modeButton;

    void sliderValueChanged (Slider* slider) override;
    void buttonClicked (Button*) override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StringUI)
};


