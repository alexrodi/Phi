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

class StringUI    : public ModuleUI
{
public:
    StringUI(ModuleProcessor&);
    ~StringUI();

    void paint (Graphics&) override;
    void wasResized(Rectangle<int> moduleBounds) override;

private:
    ui_Dial frequencyDial, positionDial, dampDial, decayDial;
    ui_SliderButton modeButton;
    SliderParameterAttachment frequencyAttachment, positionAttachment, dampAttachment, decayAttachment;
    ButtonParameterAttachment modeAttachment;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StringUI)
};


