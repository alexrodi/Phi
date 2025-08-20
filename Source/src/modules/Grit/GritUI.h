/*
  ==============================================================================

    GritUI.h
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

class GritUI    : public ModuleUI
{
public:
    GritUI(ModuleProcessor& processor) :
    ModuleUI({
        // All modules must initialize these properties
        .name =  "Grit",
        .inlets = {"In", "Amount", "Density", "Freq", "Q"},
        .outlets = {"Out"},
        .width = 320,
        .height = 130,
        .minimumHeight = 100,
        .processor = processor
    }),
    noiseDial(*processor.params.getParameter("amount")),
    densityDial(*processor.params.getParameter("density")),
    freqDial(*processor.params.getParameter("center-freq")),
    qDial(*processor.params.getParameter("q"))
    {
        addAndMakeVisible(noiseDial);
        addAndMakeVisible(densityDial);
        addAndMakeVisible(freqDial);
        addAndMakeVisible(qDial);
    }
    
    ~GritUI() {};

    void paint (Graphics& g) override {};
    
    void onResize(Rectangle<int> moduleBounds) override
    {
        int dialWidth = moduleBounds.getWidth() / 4;
        
        noiseDial.setBounds( moduleBounds.removeFromLeft(dialWidth) );
        densityDial.setBounds( moduleBounds.removeFromLeft(dialWidth) );
        freqDial.setBounds( moduleBounds.removeFromLeft(dialWidth) );
        qDial.setBounds( moduleBounds );
    }

private:
    PhiDial noiseDial, densityDial, freqDial, qDial;
};
