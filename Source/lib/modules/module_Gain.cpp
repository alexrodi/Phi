/*
  ==============================================================================

    Gain.cpp
    Created: 29 Mar 2020 3:14:32pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#include <JuceHeader.h>
#include "module_Gain.h"

module_Gain::module_Gain() :
Module{{
    // All modules must initialize these properties
    .name =  "Gain",
    .inletNumber = 1,
    .outletNumber = 1,
    .width = 150,
    .height = 150,
    .minimumHeight = 100
}},
gainDial(-70, 12, " dB", 2, this)
{

    addAndMakeVisible(gainDial);
    
}

module_Gain::~module_Gain()
{
    
}

//==============================================================================
void module_Gain::paint (Graphics& g)
{

}

void module_Gain::wasResized(Rectangle<int> moduleBounds)
{
    gainDial.setBounds( moduleBounds );
}

void module_Gain::sliderValueChanged (Slider* slider)
{
    if (slider == &gainDial){
        gainValue = (float) gainDial.getValue();
        repaint();
    }
}

//==============================================================================

void module_Gain::prepareToPlay (double sampleRate, int maximumExpectedSamplesPerBlock)
{
    setPlayConfigDetails (props.inletNumber, props.outletNumber, sampleRate, maximumExpectedSamplesPerBlock);
}

void module_Gain::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    // We can do this because the number of inputs
    // matches the number of outputs, otherwise
    // we would have to use the overload (channel, startSample, numSamples, gain)
    buffer.applyGain(gainValue);
}

void module_Gain::releaseResources()
{
}

