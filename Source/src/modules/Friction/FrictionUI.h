/*
  ==============================================================================

    FrictionUI.h
    Created: 20 Aug 2025 1:56:03am
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

///@cond
#include <JuceHeader.h>
///@endcond
#include "../../ModuleProcessor.h"
#include "../../ui/PhiDial.h"

class FrictionUI    : public ModuleUI
{
public:
    FrictionUI(ModuleProcessor& processor) :
    ModuleUI({
        // All modules must initialize these properties
        .name =  "Friction",
        .inlets = {"In"},
        .outlets = {"Out"},
        .width = 260,
        .height = 130,
        .minimumHeight = 100,
        .processor = processor
    }),
    noiseDial(*processor.params.getParameter("amount")),
    densityDial(*processor.params.getParameter("density")),
    cutoffDial(*processor.params.getParameter("cutoff"))
    {
        addAndMakeVisible(noiseDial);
        addAndMakeVisible(densityDial);
        addAndMakeVisible(cutoffDial);
    }
    
    ~FrictionUI() {};

    void paint (Graphics& g) override {};
    
    void onResize(Rectangle<int> moduleBounds) override
    {
        int dialWidth = moduleBounds.getWidth() / 3;
        
        noiseDial.setBounds( moduleBounds.removeFromLeft(dialWidth) );
        densityDial.setBounds( moduleBounds.removeFromLeft(dialWidth) );
        cutoffDial.setBounds( moduleBounds );
    }

private:
    PhiDial noiseDial, densityDial, cutoffDial;
};
