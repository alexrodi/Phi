/*
  ==============================================================================

    FrictionProcessor.h
    Created: 20 Aug 2025 1:56:03am
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

///@cond
#include <JuceHeader.h>
///@endcond
#include "FrictionUI.h"

//==============================================================================
/*
*/
class FrictionProcessor : public ModuleProcessor
{
    Random rng;;
    std::vector<float> noiseSamples;
    IIRFilter filter;
    
    double sampleRate = 44100.0;
    
public:
    FrictionProcessor() :
    ModuleProcessor(
        1, // Inlets
        1, // Outlets
        //============= Parameters =============
        std::make_unique<FloatParameter> (
            "amount",
            "Amount",
            NormalisableRange<float> (0.0, 100.0f, 0.0f, 0.5f),
            50.0f,
            FloatParameter::Attributes{}.withLabel("%")
        ),
        std::make_unique<FloatParameter> (
           "density",
           "Density",
           NormalisableRange<float> (0.0f, 100.0f, 0.0f, 0.3f),
           50.0f,
           FloatParameter::Attributes{}.withLabel("%")
        ),
        std::make_unique<FloatParameter> (
            "cutoff",
            "Cutoff",
            NormalisableRange<float> (20.0f, 20000.0f, 0.0, 0.2f),
            1000.0f,
            FloatParameter::Attributes{}.withLabel("Hz")
        )
    )
    {}
    
    ~FrictionProcessor() {}
    
    void prepare (double newSampleRate, int maxBlockSize) override {
        sampleRate = newSampleRate;
        noiseSamples.resize(maxBlockSize);
    }
    
    void process (AudioBuffer<float>& buffer, MidiBuffer&) override
    {
        float noiseGain = *params.getRawParameterValue("amount") * 0.01f;
        float density = *params.getRawParameterValue("density") * 0.01f;
        
        for (float& sample : noiseSamples)
            sample = (rng.nextFloat() < density) ? rng.nextBool() ? 0.5f : -0.5f : 0.0f;
        
        filter.setCoefficients(IIRCoefficients::makeLowPass(sampleRate, *params.getRawParameterValue("cutoff")));
        
        filter.processSamples(noiseSamples.data(), buffer.getNumSamples());
        
        FloatVectorOperations::multiply(noiseSamples.data(), noiseGain, buffer.getNumSamples());
        
        FloatVectorOperations::add(noiseSamples.data(), 1.0f - noiseGain, buffer.getNumSamples());
        
        FloatVectorOperations::multiply(buffer.getWritePointer(0), noiseSamples.data(), buffer.getNumSamples());
    }
    
    AudioProcessorEditor* createEditor() override {return new FrictionUI(*this);}
};
