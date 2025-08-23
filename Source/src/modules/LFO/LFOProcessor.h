/*
  ==============================================================================

    LFOProcessor.h
    Created: 20 Aug 2025 15:46:03am
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

#include "LFO.h"
#include "LFOUI.h"

//==============================================================================
/*
*/
struct LFOProcessor : ModuleProcessor
{
    LFOProcessor() :
    ModuleProcessor(
        2, // Inlets
        1, // Outlets
        //============= Parameters =============
        std::make_unique<FloatParameter> (
            "rate",
            "Rate",
            juce::NormalisableRange<float> (0.0f, 1.0f),
            0.3f,
            FloatParameter::Attributes{}.withStringFromValueFunction([] (float value, int) {
                return juce::String(LFO::rate_to_hz(value), 2) + " Hz";
            })
        ),
        std::make_unique<juce::AudioParameterChoice> (
            "wave",
            "Wave",
            LFO::WAVE_STRINGS,
            0
        ),
        std::make_unique<FloatParameter> (
            "shape",
            "Shape",
            juce::NormalisableRange<float> (-100.0f, 100.0f),
            0.0f,
            FloatParameter::Attributes{}.withLabel("%")
        )
    )
    {}
    
    ~LFOProcessor() {}
    
    void prepare (double newSampleRate, int maxBlockSize) override {
        lfo.prepare(newSampleRate);
    }
    
    void process (juce::AudioBuffer<float>& buffer, juce::MidiBuffer&) override
    {
        float* samples = buffer.getWritePointer(0);
        const float* rateCVSamples = buffer.getReadPointer(0);
        const float* shapeCVSamples = buffer.getReadPointer(1);

        for (int n = 0; n < buffer.getNumSamples(); n++)
        {
            lfo.set_rate(clip(rate + *rateCVSamples++, 0.0f, 1.0f));
            lfo.set_shape(clip(shape + *shapeCVSamples++, 0.0f, 1.0f));
            *samples++ = lfo.next();
        }
        
        
//        float* samples = buffer.getWritePointer(0);
//        const float* freqCVSamples = buffer.getReadPointer(0);
//        const float* shapeCVSamples = buffer.getReadPointer(1);
//        const float* cutoffCVSamples = buffer.getReadPointer(2);
//        
//        for (int n = 0; n < buffer.getNumSamples(); n++)
//        {
//            float saw = sawtooth.process(std::min(freq * pow(5.0f, *freqCVSamples++), 20000.0f), clip(shape + *shapeCVSamples++, -1.0f, 1.0f));
//            
//            filter.setCoefficients(IIRCoefficients::makeLowPass(sampleRate, std::min(cutoff * pow(5.0f, *cutoffCVSamples++), 20000.0f)));
//            
//            *samples++ = filter.processSingleSampleRaw(saw);;
//        }
    }
    
    void parameterChanged (const juce::String& parameterID, float value) override {
        if (parameterID == "rate") rate = value;
        else if (parameterID == "wave") lfo.set_wave((LFO::Wave)floor(value));
        else if (parameterID == "shape") shape = value * 0.01f;
    }
    
    std::unique_ptr<ModuleUI> createUI() override { return std::make_unique<LFOUI>(*this); }
    
private:
    LFO lfo;
    float rate = 0.0f, shape = 1.0f;
};
