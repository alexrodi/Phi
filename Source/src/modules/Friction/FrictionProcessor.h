/*
  ==============================================================================

    FrictionProcessor.h
    Created: 20 Aug 2025 12:42:03am
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
struct FrictionProcessor : ModuleProcessor
{
    FrictionProcessor() :
    ModuleProcessor(
        3, // Inlets
        1, // Outlets
        //============= Parameters =============
        std::make_unique<FloatParameter> (
            "freq",
            "Freq",
            NormalisableRange<float> (0.1f, 10000.0f, 0.0, 0.2f),
            30.0f,
            FloatParameter::Attributes{}.withLabel("Hz")
        ),
        std::make_unique<FloatParameter> (
           "shape",
           "Shape",
           NormalisableRange<float> (-100.0f, 100.0f, 0.0f, 0.3f, true),
           0.0f,
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
        sawtooth.prepare(newSampleRate);
    }
    
    void process (AudioBuffer<float>& buffer, MidiBuffer&) override
    {
        float* samples = buffer.getWritePointer(0);
        const float* freqCVSamples = buffer.getReadPointer(0);
        const float* shapeCVSamples = buffer.getReadPointer(1);
        const float* cutoffCVSamples = buffer.getReadPointer(2);
        
        for (int n = 0; n < buffer.getNumSamples(); n++)
        {
            float saw = sawtooth.process(std::min(freq * pow(5.0f, *freqCVSamples++), 20000.0f), clip(shape + *shapeCVSamples++, -1.0f, 1.0f));
            
            filter.setCoefficients(IIRCoefficients::makeLowPass(sampleRate, std::min(cutoff * pow(5.0f, *cutoffCVSamples++), 20000.0f)));
            
            *samples++ = filter.processSingleSampleRaw(saw);;
        }
    }
    
    void parameterChanged (const String& parameterID, float value) override {
        if (parameterID == "freq") freq = value;
        else if (parameterID == "shape") shape = value * 0.01f;
        else if (parameterID == "cutoff") cutoff = value;
    }
    
    AudioProcessorEditor* createEditor() override {return new FrictionUI(*this);}
    
private:
    struct SawtoothGenerator {
        void prepare(double newSampleRate) {
            incrFactor = 1.0 / newSampleRate;
            dcBlock.prepare(newSampleRate);
            phase = 0.0f;
        }
        
        /// freq: [0 , 20000]
        /// shape: [-1, 1]
        float process (float freq, float shape) {
            double increment = (double)freq * incrFactor;
            
            float saw = 2.0f * phase - 1.0f;
            
            saw -= blep(increment, phase);
            
            saw = pow(saw * 0.5f + 0.5f, pow(10.0f, shape)) * 2.0f - 1.0f; // Waveshaping
            
            saw = dcBlock.process(saw);
            
            phase += increment;
            
            if (phase >= 1.0)
                phase -= 1.0;
            
            return saw;
        }
        
    private:
        double phase = 0.0, incrFactor = 0.01;
        DCBlock<float> dcBlock;
        
        // A simple 3rd-order polynomial approximation for a BLEP.
        static constexpr float blep (float dt, float t) {
            if (t < dt) {
                t /= dt;
                return t * (2.0f - t) - 1.0f;
            } else if (t > 1.0f - dt) {
                t = (t - 1.0f) / dt;
                return t * (t + 2.0f) + 1.0f;
            }
            return 0.0f;
        }
    } sawtooth;
    
    IIRFilter filter;
    float freq = 30.0f, shape = 0.0f, cutoff = 20.0f;
    double sampleRate = 44100.0;
};
