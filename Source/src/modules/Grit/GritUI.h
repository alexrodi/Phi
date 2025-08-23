/*
  ==============================================================================

    GritUI.h
    Created: 20 Aug 2025 1:56:03am
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

#include "../../ModuleProcessor.h"
#include "../../ui/component/PhiDial.h"

class GritUI    : public ModuleUI
{
public:
    GritUI(ModuleProcessor& processor) :
    ModuleUI({
        // All modules must initialize these properties
        .name =  "Grit",
        .inlets = {"In", "Amount", "Density"},
        .outlets = {"Out"},
        .width = 230,
        .height = 130,
        .minimumHeight = 100,
        .processor = processor
    }),
    noiseDial(*processor.params.getParameter("amount")),
    densityDial(*processor.params.getParameter("density"))
    {
        addAndMakeVisible(noiseDial);
        addAndMakeVisible(densityDial);
    }
    
    ~GritUI() {};

    void paint (juce::Graphics& g) override {};
    
    void onResize(juce::Rectangle<int> moduleBounds) override
    {
        int dialWidth = moduleBounds.getWidth() / 2;
        
        noiseDial.setBounds( moduleBounds.removeFromLeft(dialWidth) );
        densityDial.setBounds( moduleBounds );
    }

private:
    PhiDial noiseDial, densityDial;
};
