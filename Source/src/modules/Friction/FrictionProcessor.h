/*
  ==============================================================================

    FrictionProcessor.h
    Created: 20 Aug 2025 12:42:03am
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

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
            juce::NormalisableRange<float> (0.1f, 10000.0f, 0.0, 0.2f),
            30.0f,
            FloatParameter::Attributes{}.withLabel("Hz")
        ),
        std::make_unique<FloatParameter> (
            "jitter",
            "Jitter",
            juce::NormalisableRange<float> (0.0f, 100.0f, 0.0f, 0.3f),
            0.0f,
            FloatParameter::Attributes{}.withLabel("%")
        ),
        std::make_unique<FloatParameter> (
            "drift",
            "Drift",
            juce::NormalisableRange<float> (0.0f, 95.0f, 0.0, 0.5f),
            0.0f,
            FloatParameter::Attributes{}.withLabel("%")
        )
    )
    {}
    
    ~FrictionProcessor() {}
    
    void prepare (double newSampleRate, int maxBlockSize) override {
        sampleRate = newSampleRate;
        sawtooth.prepare(newSampleRate);
    }
    
    void process (juce::AudioBuffer<float>& buffer, juce::MidiBuffer&) override
    {
        float* samples = buffer.getWritePointer(0);
        const float* freqCVSamples = buffer.getReadPointer(0);
        const float* jitterCVSamples = buffer.getReadPointer(1);
        const float* driftCVSamples = buffer.getReadPointer(2);
        
        for (int n = 0; n < buffer.getNumSamples(); n++) {
            *samples++ = sawtooth.process(
                std::min(freq * pow(5.0f, *freqCVSamples++), 20000.0f),
                clip(jitter + *jitterCVSamples++, 0.0f, 1.0f),
                clip(drift + *driftCVSamples++, 0.0f, 1.0f)
            );
        }
    }
    
    void parameterChanged (const juce::String& parameterID, float value) override {
        if (parameterID == "freq") freq = value;
        else if (parameterID == "jitter") jitter = value * 0.01f;
        else if (parameterID == "drift") drift = value * 0.01f;
    }
    
    std::unique_ptr<ModuleUI> createUI() override { return std::make_unique<FrictionUI>(*this); }
    
private:
    struct SawtoothGenerator {
        void prepare(double newSampleRate) {
            incrFactor = 1.0 / newSampleRate;
            phase = 0.0f;
        }
        
        float nextDriftValue() {
            float f = driftValue + (rng.nextFloat() - 0.5f) * 0.1f;
            
            // Fold
            if (f >= 1.0f)
                f = 1.0f - (f - 1.0);
            else if (f <= -1.0f)
                f = -1.0 - (f + 1.0);
            
            return f;
        }

        float process (float freq, float jitter, float drift) {
            double increment = (double)freq * incrFactor * jitterFactor * (1.0f + driftValue * drift);
            
            float saw = 2.0f * phase - 1.0f;
            
            saw -= blep(increment, phase);
            
            phase += increment;
            
            if (phase >= 1.0) {
                // jitter changes the frequency of the next period
                jitterFactor = pow(4.0, jitter * (rng.nextFloat() - 0.5f));
                
                // drift shifts the frequency in a random walk
                driftValue = nextDriftValue();
                
                phase -= 1.0;
            }
            
            return saw;
        }
        
    private:
        juce::Random rng;
        double phase = 0.0, incrFactor = 0.01, jitterFactor = 1.0, driftValue = 1.0;
        
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
    
    float freq = 30.0f, jitter = 0.0f, drift = 0.0f;
    double sampleRate = 44100.0;
};
