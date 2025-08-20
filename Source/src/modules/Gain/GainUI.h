/*
  ==============================================================================

    Gain.h
    Created: 29 Mar 2020 3:14:32pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

///@cond
#include <JuceHeader.h>
///@endcond
#include "../../ModuleProcessor.h"
#include "../../ui/PhiDial.h"

class GainUI    : public ModuleUI
{
public:
    GainUI(ModuleProcessor& processor) :
    ModuleUI({
        // All modules must initialize these properties
        .name =  "Gain",
        .inlets = {"In"},
        .outlets = {"Out"},
        .width = 160,
        .height = 160,
        .minimumHeight = 100,
        .processor = processor
    }),
    gainDial(*processor.params.getParameter("gain"))
    {
        addAndMakeVisible(gainDial);
    }
    
    ~GainUI() {};

    void paint (Graphics& g) override {};
    
    void onResize(Rectangle<int> moduleBounds) override
    {
        gainDial.setBounds( moduleBounds );
    }

private:
    PhiDial gainDial;
};
