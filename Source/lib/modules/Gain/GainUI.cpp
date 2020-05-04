/*
  ==============================================================================

    GainUI.cpp
    Created: 25 Apr 2020 11:55:34pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#include "GainUI.h"


GainUI::GainUI(GainProcessor& processor) :
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
gainDial("Gain", -70, 12, this, 1, " dB")
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


void GainUI::sliderValueChanged (Slider* slider)
{
    if (slider == &gainDial){
        *props.processor.params.getRawParameterValue("gain") = Decibels::decibelsToGain(gainDial.getValue());
        repaint();
    }
}
