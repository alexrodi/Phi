/*
  ==============================================================================

    FrictionUI.h
    Created: 20 Aug 2025 12:42:03am
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

#include "../../ui/ModuleUI.h"
#include "../../ui/component/PhiDial.h"

struct FrictionUI : ModuleUI
{
    FrictionUI(ModuleProcessor& processor) :
    ModuleUI({
        // All modules must initialize these properties
        .name =  "Friction",
        .inlets = {"Freq", "Jitter", "Drift"},
        .outlets = {"Out"},
        .defaultSize = {270, 130},
        .minimumSize = {233, 108},
        .processor = processor
    }),
    freqDial(*processor.params.getParameter("freq")),
    jitterDial(*processor.params.getParameter("jitter")),
    driftDial(*processor.params.getParameter("drift"))
    {
        addAndMakeVisible(freqDial);
        addAndMakeVisible(jitterDial);
        addAndMakeVisible(driftDial);
    }
    
    ~FrictionUI() {};

    void paint (juce::Graphics& g) override {};
    
    void resized() override
    {
        auto bounds = getLocalBounds();
        
        int dialWidth = bounds.getWidth() / 3;
        
        freqDial.setBounds( bounds.removeFromLeft(dialWidth) );
        jitterDial.setBounds( bounds.removeFromLeft(dialWidth) );
        driftDial.setBounds( bounds );
    }

private:
    PhiDial freqDial, jitterDial, driftDial;
};
