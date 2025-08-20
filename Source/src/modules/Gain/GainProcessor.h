/*
  ==============================================================================

    GainProcessor.h
    Created: 21 Apr 2020 8:18:03pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

///@cond
#include <JuceHeader.h>
///@endcond
#include "GainUI.h"

//==============================================================================
/*
*/
class GainProcessor    : public ModuleProcessor
{
public:
    GainProcessor() :
    ModuleProcessor(
        1, // Inlets
        1, // Outlets
        //============= Parameters =============
        std::make_unique<FloatParameter> (
            "gain",
            "Gain",
            NormalisableRange<float> (-70.0f, 12.0f),
            0.0f,
            FloatParameter::Attributes{}.withLabel("dB")
        )
    )
    {}
    
    ~GainProcessor() {}
    
    void prepare (double sampleRate, int maxBlockSize) override {}
    
    void process (AudioBuffer<float>& buffer, MidiBuffer&) override
    {
       buffer.applyGain(juce::Decibels::decibelsToGain((float)*params.getRawParameterValue("gain")));
    }
    
    AudioProcessorEditor* createEditor() override {return new GainUI(*this);}
};
