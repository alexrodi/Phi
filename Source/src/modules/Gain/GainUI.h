/*
  ==============================================================================

    Gain.h
    Created: 29 Mar 2020 3:14:32pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

#include "../../ui/ModuleUI.h"
#include "../../ui/component/PhiDial.h"

class GainUI    : public ModuleUI
{
public:
    GainUI(ModuleProcessor& processor) :
    ModuleUI({
        // All modules must initialize these properties
        .name =  "Gain",
        .inlets = {"In", "Gain"},
        .outlets = {"Out"},
        .width = 160,
        .height = 160,
        .minimumHeight = 100,
        .processor = processor
    }),
    gainDial(*processor.params.getParameter("gain"))
    {
        addAndMakeVisible(gainDial);
    }
    
    ~GainUI() {};

    void paint (juce::Graphics& g) override {};
    
    void resized() override
    {
        gainDial.setBounds(getLocalBounds());
    }

private:
    PhiDial gainDial;
};
