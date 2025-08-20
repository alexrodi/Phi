/*
  ==============================================================================

    LFOProcessor.h
    Created: 20 Aug 2025 15:46:03am
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

///@cond
#include <JuceHeader.h>
///@endcond
#include "LFO.h"
#include "LFOUI.h"

//==============================================================================
/*
*/
struct LFOProcessor : ModuleProcessor
{
    LFOProcessor() :
    ModuleProcessor(
        0, // Inlets
        1, // Outlets
        //============= Parameters =============
        std::make_unique<FloatParameter> (
            "rate",
            "Rate",
            NormalisableRange<float> (0.0f, 1.0f),
            0.3f,
            FloatParameter::Attributes{}.withStringFromValueFunction([] (float value, int) {
                return juce::String(LFO::rate_to_hz(value), 2) + " Hz";
            })
        ),
        std::make_unique<AudioParameterChoice> (
            "wave",
            "Wave",
            LFO::WAVE_STRINGS,
            0
        ),
        std::make_unique<FloatParameter> (
            "shape",
            "Shape",
            NormalisableRange<float> (-100.0f, 100.0f),
            0.0f,
            FloatParameter::Attributes{}.withLabel("%")
        )
    )
    {}
    
    ~LFOProcessor() {}
    
    void prepare (double newSampleRate, int maxBlockSize) override {
        lfo.prepare(newSampleRate);
    }
    
    void process (AudioBuffer<float>& buffer, MidiBuffer&) override
    {
        float* samples = buffer.getWritePointer(0);

        for (int n = 0; n < buffer.getNumSamples(); n++)
        {
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
    
    void parameterChanged (const String& parameterID, float value) override {
        if (parameterID == "rate") lfo.set_rate(value);
        else if (parameterID == "wave") lfo.set_wave((LFO::Wave)floor(value));
        else if (parameterID == "shape") lfo.set_shape(value * 0.01f);
    }
    
    AudioProcessorEditor* createEditor() override {return new LFOUI(*this);}
    
private:
    LFO lfo;
};
