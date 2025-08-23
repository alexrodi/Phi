/*
  ==============================================================================

    FrictionUI.h
    Created: 20 Aug 2025 12:42:03am
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

#include "../../ModuleProcessor.h"
#include "../../ui/component/PhiDial.h"

struct FrictionUI : ModuleUI
{
    FrictionUI(ModuleProcessor& processor) :
    ModuleUI({
        // All modules must initialize these properties
        .name =  "Friction",
        .inlets = {"Freq", "Jitter", "Drift"},
        .outlets = {"Out"},
        .width = 270,
        .height = 130,
        .minimumHeight = 100,
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
    
    void onResize(juce::Rectangle<int> moduleBounds) override
    {
        int dialWidth = moduleBounds.getWidth() / 3;
        
        freqDial.setBounds( moduleBounds.removeFromLeft(dialWidth) );
        jitterDial.setBounds( moduleBounds.removeFromLeft(dialWidth) );
        driftDial.setBounds( moduleBounds );
    }

private:
    PhiDial freqDial, jitterDial, driftDial;
};
