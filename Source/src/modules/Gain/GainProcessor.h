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
        2, // Inlets
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
        float gain = juce::Decibels::decibelsToGain(params.getRawParameterValue("gain")->load());
        float* inOutSamples = buffer.getWritePointer(0);
        const float* gainCVSamples = buffer.getReadPointer(1);
        
        for (int n = 0; n < buffer.getNumSamples(); n++)
            *inOutSamples++ *= clip(gain + *gainCVSamples++, 0.0f, 4.0f);
    }
    
    AudioProcessorEditor* createEditor() override {return new GainUI(*this);}
};
