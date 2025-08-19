/*
  ==============================================================================

    ImpusleProcessor.h
    Created: 25 Apr 2020 12:10:22am
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

///@cond
#include <JuceHeader.h>
///@endcond

// Definition is global for the UI to build a faithful representation
static constexpr float processImpulse(float phase, float shape)
{
    const float shapeFactor = -std::max(shape, 0.88f) + 1.01f;
    const float fundamentalAttenuator = -0.5 * tanh( phase * shapeFactor - 1.0f ) + 0.5;

    return (phase == MathConstants<float>::pi)
           ? 0.0f
           : sin((sin(phase))/((-shape + 1.006f)*(phase-MathConstants<float>::pi)))*fundamentalAttenuator;
}

#include "ImpulseUI.h"

class ImpulseProcessor : public ModuleProcessor
{
public:
    static const float constexpr invTwoPi = 1.0f/MathConstants<float>::twoPi;
    
    ImpulseProcessor() :
    ModuleProcessor(
        3, // Inlets
        2, // Outlets
        //============= Parameters =============
        std::make_unique<AudioParameterFloat> (
           "freq",
           "Frequency",
           NormalisableRange<float> (20.0f, 20000.0f, 0, 0.2f),
           1000.0f,
           "Frequency",
           AudioParameterFloat::genericParameter,
           [](float value, int) { return String (value, 1); },
           [](const String& text) { return text.getFloatValue(); }
        ),
        std::make_unique<AudioParameterFloat> (
           "shape",
           "Shape",
           NormalisableRange<float> (0.0f, 1.0f),
           0.0f,
           "Shape",
           AudioParameterFloat::genericParameter,
           [](float value, int) { return String (floorf(value * 100.0f), 0); },
           [](const String& text) { return text.getFloatValue() * 0.01f; }
        ),
        std::make_unique<AudioParameterBool> (
           "trigger",
           "Trigger",
           false
        )
    )
    {}
    
    ~ImpulseProcessor() {}
    
    void prepareToPlay (double newSampleRate, int maxBlockSize) override
    {
        incrFactor = MathConstants<float>::twoPi / (float)newSampleRate;
    }
    
    void processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages) override
    {
        const float phaseIncrement = freq * incrFactor;
        const float* triggerSamples = buffer.getReadPointer(0);
        const float* freqCVSamples = buffer.getReadPointer(1);
        const float* shapeCVSamples = buffer.getReadPointer(2);
        float* outSamples = buffer.getWritePointer(0);
        float* rampSamples = buffer.getWritePointer(1);
        
        if (triggerParameterWasOn()) currentPhase = 0.0f;
        
        // for now, we constantly process audio, but ideally, the module should know if it is connected
        for (int n = 0; n < buffer.getNumSamples(); n++)
        {
            const float trigger = *triggerSamples++;
            if ((previousTrigger - trigger) > 0.5f) currentPhase = 0.0f;
            
            float nextPhase = currentPhase + phaseIncrement * pow(5.0f, *freqCVSamples++);
            
            *outSamples++ = processImpulse(currentPhase, clip(shape + *shapeCVSamples++, 0.0f, 1.0f));
            *rampSamples++ = std::min(1.0f, currentPhase * invTwoPi);

            currentPhase = nextPhase;
            currentPhase = std::min(std::numeric_limits<float>::max(), currentPhase);
            previousTrigger = trigger;
        }
    }
    
    void parameterChanged (const String& parameterID, float value) override {
        if (parameterID == "freq") freq = value;
        else if (parameterID == "shape") shape = pow(value, 0.1f);
    }

    AudioProcessorEditor* createEditor() override { return new ImpulseUI(*this); }
    
private:
    float incrFactor = 1.0f;
    float currentPhase = 0.0f;
    float previousTrigger = 0.0f;
    float freq = 1.0f, shape = 0.0f;
    
    bool triggerParameterWasOn()
    {
        return params.getRawParameterValue("trigger")->exchange(0.0f) > 0.0f;
    }
};
