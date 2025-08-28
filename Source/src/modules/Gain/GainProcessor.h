/*
  ==============================================================================

    GainProcessor.h
    Created: 21 Apr 2020 8:18:03pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

#include "GainUI.h"
#include "../../dsp/ModuleProcessor.h"

//==============================================================================
/*
*/
struct GainProcessor : ModuleProcessor
{
    GainProcessor() :
    ModuleProcessor(
        2, // Inlets
        1, // Outlets
        //============= Parameters =============
        std::make_unique<FloatParameter> (
            "gain",
            "Gain",
                                          juce::NormalisableRange<float> (-70.0f, 12.0f),
            0.0f,
            FloatParameter::Attributes{}.withLabel("dB")
        )
    )
    {}
    
    ~GainProcessor() {}
    
    void prepare (double sampleRate, int maxBlockSize) override {}
    
    void process (juce::AudioBuffer<float>& buffer, juce::MidiBuffer&) override
    {
        float gain = db_to_a(params.getRawParameterValue("gain")->load());
        float* inOutSamples = buffer.getWritePointer(0);
        const float* gainCVSamples = buffer.getReadPointer(1);
        
        for (int n = 0; n < buffer.getNumSamples(); n++)
            *inOutSamples++ *= clip(gain + *gainCVSamples++, 0.0f, 4.0f);
    }
    
    std::unique_ptr<ModuleUI> createUI() override { return std::make_unique<GainUI>(*this); }
};
