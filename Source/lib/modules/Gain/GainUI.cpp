/*
  ==============================================================================

    GainUI.cpp
    Created: 25 Apr 2020 11:55:34pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#include "GainUI.h"


GainUI::GainUI(ModuleProcessor& processor) :
ModuleUI({
            // All modules must initialize these properties
            .name =  "Gain",
            .inlets = {"In"},
            .outlets = {"Out"},
            .width = 150,
            .height = 150,
            .minimumHeight = 100,
            .processor = processor
        }),
gainDial("Gain", -70, 12, 1, " dB"),
gainAttachment(*processor.params.getParameter("gain"), gainDial)
{
    addAndMakeVisible(gainDial);
}
GainUI::~GainUI()
{
}

void GainUI::paint (Graphics&)
{
    
}
void GainUI::wasResized(Rectangle<int> moduleBounds)
{
    gainDial.setBounds( moduleBounds );
}
