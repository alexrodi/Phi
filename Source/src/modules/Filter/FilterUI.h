/*
  ==============================================================================

    FilterUI.h
    Created: 21 Aug 2025 12:24:03am
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

#include "../../ModuleProcessor.h"
#include "../../ui/component/PhiDial.h"

struct FilterUI : ModuleUI
{
    FilterUI(ModuleProcessor& processor) :
    ModuleUI({
        // All modules must initialize these properties
        .name =  "Filter",
        .inlets = {"In", "Freq", "Res"},
        .outlets = {"Low", "Band", "High"},
        .width = 230,
        .height = 130,
        .minimumHeight = 100,
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
    
    void onResize(juce::Rectangle<int> moduleBounds) override
    {
        int dialWidth = moduleBounds.getWidth() / 2;
        
        freqDial.setBounds( moduleBounds.removeFromLeft(dialWidth) );
        resDial.setBounds( moduleBounds );
    }

private:
    PhiDial freqDial, resDial;
};
