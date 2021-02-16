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
#include "../../ModuleProcessor.h"
#include "../../ui/PhiButton.h"

class TriggerUI    : public ModuleUI
{
public:
    TriggerUI(ModuleProcessor& processor) :
    ModuleUI({
        // All modules must initialize these properties
        .name =  "Trigger",
        .inlets = {},
        .outlets = {"Out"},
        .width = 105,
        .height = 100,
        .minimumHeight = 90,
        .processor = processor
    }),
    triggerButton(PhiButton::Momentary),
    triggerAttachment(*processor.params.getParameter("trigger"), triggerButton)
    {
        addAndMakeVisible(triggerButton);
    }
    
    ~TriggerUI() {};

    void paint (Graphics& g) override {};
    
    void wasResized(Rectangle<int> moduleBounds) override
    {
        triggerButton.setBounds( moduleBounds );
    }

private:
    PhiButton triggerButton;
    ButtonParameterAttachment triggerAttachment;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TriggerUI)
};
