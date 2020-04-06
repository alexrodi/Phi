/*
  ==============================================================================

    module_Output.cpp
    Created: 5 Apr 2020 10:13:32pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#include <JuceHeader.h>
#include "module_Output.h"

//==============================================================================
module_Output::module_Output() :
Module{{
    // All modules must initialize these properties
    .name =  "Output",
    .inletNumber = 2,
    .outletNumber = 0,
    .width = 150,
    .height = 150,
    .minimumHeight = 100
}}
{
    // module_Output cheats on the number of output channels
    // these are only used internally for audio and are not displayed
    setPlayConfigDetails (props.inletNumber, props.inletNumber, getSampleRate(), getBlockSize());
}

module_Output::~module_Output()
{
}

void module_Output::paint (Graphics& g)
{
}

void module_Output::wasResized(Rectangle<int>)
{
}

//==============================================================================

void module_Output::prepareToPlay (double sampleRate, int maximumExpectedSamplesPerBlock)
{
}

void module_Output::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
}

void module_Output::releaseResources()
{
}
