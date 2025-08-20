/*
  ==============================================================================

    GritProcessor.h
    Created: 20 Aug 2025 1:56:03am
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

///@cond
#include <JuceHeader.h>
///@endcond
#include "GritUI.h"

//==============================================================================
/*
*/
class GritProcessor : public ModuleProcessor
{
    Random rng;
    IIRFilter filter;
    
    float amount = 0.0f, density = 0.0f, freq = 20.0f, q = 1.0f;
    double sampleRate = 44100.0;
    
public:
    GritProcessor() :
    ModuleProcessor(
        5, // Inlets
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
            "center-freq",
            "Freq",
            NormalisableRange<float> (20.0f, 20000.0f, 0.0, 0.2f),
            1000.0f,
            FloatParameter::Attributes{}.withLabel("Hz")
        ),
        std::make_unique<FloatParameter> (
            "q",
            "Q",
            NormalisableRange<float> (0.1f, 18.0f, 0.0, 0.2f),
            1.0f
        )
    )
    {}
    
    ~GritProcessor() {}
    
    void prepare (double newSampleRate, int maxBlockSize) override {
        sampleRate = newSampleRate;
    }
    
    void process (AudioBuffer<float>& buffer, MidiBuffer&) override
    {
        float* inOutSamples = buffer.getWritePointer(0);
        const float* amountCVSamples = buffer.getReadPointer(1);
        const float* densityCVSamples = buffer.getReadPointer(2);
        const float* freqCVSamples = buffer.getReadPointer(3);
        const float* qCVSamples = buffer.getReadPointer(4);
        
        for (int n = 0; n < buffer.getNumSamples(); n++)
        {
            float thresh = density + *densityCVSamples++;
            float noise = (rng.nextFloat() < thresh) ? rng.nextBool() ? 0.5f : -0.5f : 0.0f;
            
            filter.setCoefficients(IIRCoefficients::makeBandPass(
                sampleRate,
                std::min(freq * pow(5.0f, *freqCVSamples++), 20000.0f),
                std::max(q + bipow(*qCVSamples++, 3.0f) * 18.0f, 0.0f)
            ));
            noise = filter.processSingleSampleRaw(noise);
            
            float gain = clip(amount + *amountCVSamples++, 0.0f, 1.0f);
            noise = noise * gain + (1.0f - gain);
            
            *inOutSamples++ *= noise;
        }
    }
    
    void parameterChanged (const String& parameterID, float value) override {
        if (parameterID == "amount") amount = value * 0.01f;
        else if (parameterID == "density") density = value * 0.01f;
        else if (parameterID == "center-freq") freq = value;
        else if (parameterID == "q") q = value;
    }
    
    AudioProcessorEditor* createEditor() override {return new GritUI(*this);}
    
};
