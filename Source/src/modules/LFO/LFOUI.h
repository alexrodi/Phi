/*
  ==============================================================================

    LFOUI.h
    Created: 20 Aug 2025 15:46:03am
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

///@cond
#include <JuceHeader.h>
///@endcond
#include "../../ModuleProcessor.h"
#include "../../ui/PhiDial.h"

struct LFOUI : ModuleUI
{
    LFOUI(ModuleProcessor& processor) :
    ModuleUI({
        // All modules must initialize these properties
        .name =  "LFO",
        .inlets = {},
        .outlets = {"Out"},
        .width = 260,
        .height = 130,
        .minimumHeight = 100,
        .processor = processor
    }),
    rateDial(*processor.params.getParameter("rate")),
    waveDial(*processor.params.getParameter("wave")),
    shapeDial(*processor.params.getParameter("shape"))
    {
        addAndMakeVisible(rateDial);
        addAndMakeVisible(waveDial);
        addAndMakeVisible(shapeDial);
    }
    
    ~LFOUI() {};

    void paint (Graphics& g) override {};
    
    void onResize(Rectangle<int> moduleBounds) override
    {
        int dialWidth = moduleBounds.getWidth() / 3;
        
        rateDial.setBounds( moduleBounds.removeFromLeft(dialWidth) );
        waveDial.setBounds( moduleBounds.removeFromLeft(dialWidth) );
        shapeDial.setBounds( moduleBounds );
    }

private:
    PhiDial rateDial, waveDial, shapeDial;
};
