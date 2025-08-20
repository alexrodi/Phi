/*
  ==============================================================================

    FrictionUI.h
    Created: 20 Aug 2025 12:42:03am
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

///@cond
#include <JuceHeader.h>
///@endcond
#include "../../ModuleProcessor.h"
#include "../../ui/PhiDial.h"

struct FrictionUI : ModuleUI
{
    FrictionUI(ModuleProcessor& processor) :
    ModuleUI({
        // All modules must initialize these properties
        .name =  "Friction",
        .inlets = {"In", "Freq", "Jitter", "Cutoff"},
        .outlets = {"Out"},
        .width = 260,
        .height = 130,
        .minimumHeight = 100,
        .processor = processor
    }),
    freqDial(*processor.params.getParameter("freq")),
    jitterDial(*processor.params.getParameter("jitter")),
    cutoffDial(*processor.params.getParameter("cutoff"))
    {
        addAndMakeVisible(freqDial);
        addAndMakeVisible(jitterDial);
        addAndMakeVisible(cutoffDial);
    }
    
    ~FrictionUI() {};

    void paint (Graphics& g) override {};
    
    void onResize(Rectangle<int> moduleBounds) override
    {
        int dialWidth = moduleBounds.getWidth() / 3;
        
        freqDial.setBounds( moduleBounds.removeFromLeft(dialWidth) );
        jitterDial.setBounds( moduleBounds.removeFromLeft(dialWidth) );
        cutoffDial.setBounds( moduleBounds );
    }

private:
    PhiDial freqDial, jitterDial, cutoffDial;
};
