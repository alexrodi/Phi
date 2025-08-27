/*
  ==============================================================================

    GritUI.h
    Created: 20 Aug 2025 1:56:03am
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

#include "../../ui/ModuleUI.h"
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
        .defaultSize = {230, 130},
        .minimumSize = {194, 100},
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
    
    void resized() override
    {
        auto bounds = getLocalBounds();
        
        int dialWidth = bounds.getWidth() / 2;
        
        noiseDial.setBounds(bounds.removeFromLeft(dialWidth));
        densityDial.setBounds(bounds);
    }

private:
    PhiDial noiseDial, densityDial;
};
