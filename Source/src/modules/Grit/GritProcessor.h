/*
  ==============================================================================

    GritProcessor.h
    Created: 20 Aug 2025 1:56:03am
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

#include "GritUI.h"

//==============================================================================
/*
*/
struct GritProcessor : ModuleProcessor
{
    GritProcessor() :
    ModuleProcessor(
        3, // Inlets
        1, // Outlets
        //============= Parameters =============
        std::make_unique<FloatParameter> (
            "amount",
            "Amount",
            juce::NormalisableRange<float> (0.0, 100.0f, 0.0f, 0.5f),
            50.0f,
            FloatParameter::Attributes{}.withLabel("%")
        ),
        std::make_unique<FloatParameter> (
           "density",
           "Density",
           juce::NormalisableRange<float> (0.0f, 100.0f, 0.0f, 0.3f),
           50.0f,
           FloatParameter::Attributes{}.withLabel("%")
        )
    )
    {}
    
    ~GritProcessor() {}
    
    void prepare (double newSampleRate, int maxBlockSize) override {
        sampleRate = newSampleRate;
    }
    
    void process (juce::AudioBuffer<float>& buffer, juce::MidiBuffer&) override
    {
        float* inOutSamples = buffer.getWritePointer(0);
        const float* amountCVSamples = buffer.getReadPointer(1);
        const float* densityCVSamples = buffer.getReadPointer(2);
        
        for (int n = 0; n < buffer.getNumSamples(); n++)
        {
            float thresh = density + *densityCVSamples++;
            float noise = (rng.nextFloat() < thresh) ? rng.nextBool() ? 0.5f : -0.5f : 0.0f;
            
            float gain = clip(amount + *amountCVSamples++, 0.0f, 1.0f);
            noise = noise * gain + (1.0f - gain);
            
            *inOutSamples++ *= noise;
        }
    }
    
    void parameterChanged (const juce::String& parameterID, float value) override {
        if (parameterID == "amount") amount = value * 0.01f;
        else if (parameterID == "density") density = value * 0.01f;
    }
    
    std::unique_ptr<ModuleUI> createUI() override { return std::make_unique<GritUI>(*this); }
    
private:
    juce::Random rng;
    juce::IIRFilter filter;
    
    float amount = 0.0f, density = 0.0f;
    double sampleRate = 44100.0;
};
