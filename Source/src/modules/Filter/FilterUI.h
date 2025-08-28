/*
  ==============================================================================

    FilterUI.h
    Created: 21 Aug 2025 12:24:03am
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

#include "../../ui/ModuleUI.h"
#include "../../ui/component/PhiDial.h"

struct FilterUI : ModuleUI
{
    FilterUI(ModuleProcessor& processor) :
    ModuleUI({
        // All modules must initialize these properties
        .name =  "Filter",
        .inlets = {"In", "Freq", "Res"},
        .outlets = {"Low", "Band", "High"},
        .defaultSize = {230, 130},
        .minimumSize = {186, 100},
        .processor = processor
    }),
    freqDial(*processor.params.getParameter("freq")),
    resDial(*processor.params.getParameter("res"))
    {
        addAndMakeVisible(freqDial);
        addAndMakeVisible(resDial);
    }
    
    ~FilterUI() {};

    void paint (juce::Graphics& g) override {};
    
    void resized() override
    {
        auto bounds = getLocalBounds();
        
        int dialWidth = bounds.getWidth() / 2;
        
        freqDial.setBounds( bounds.removeFromLeft(dialWidth) );
        resDial.setBounds( bounds );
    }

private:
    PhiDial freqDial, resDial;
};
