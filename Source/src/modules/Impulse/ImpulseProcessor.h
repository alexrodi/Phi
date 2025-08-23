/*
  ==============================================================================

    ImpusleProcessor.h
    Created: 25 Apr 2020 12:10:22am
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once


// Definition is global for the UI to build a faithful representation
static constexpr float processImpulse(float phase, float shape)
{
    const float shapeFactor = -std::max(shape, 0.88f) + 1.01f;
    const float fundamentalAttenuator = -0.5 * tanh( phase * shapeFactor - 1.0f ) + 0.5;

    return (phase == juce::MathConstants<float>::pi)
           ? 0.0f
    : sin((sin(phase))/((-shape + 1.006f)*(phase-juce::MathConstants<float>::pi)))*fundamentalAttenuator;
}

#include "ImpulseUI.h"

struct ImpulseProcessor : ModuleProcessor
{
    ImpulseProcessor() :
    ModuleProcessor(
        3, // Inlets
        2, // Outlets
        //============= Parameters =============
        std::make_unique<FloatParameter> (
           "freq",
           "Frequency",
           juce::NormalisableRange<float> (20.0f, 20000.0f, 0.0, 0.2f),
           1000.0f,
           FloatParameter::Attributes{}.withLabel("Hz")
        ),
        std::make_unique<FloatParameter> (
           "shape",
           "Shape",
           juce::NormalisableRange<float> (0.0f, 100.0f),
           0.0f,
           FloatParameter::Attributes{}.withLabel("%")
        ),
        std::make_unique<juce::AudioParameterBool> (
           "trigger",
           "Trigger",
           false
        )
    )
    {}
    
    ~ImpulseProcessor() {}
    
    void prepare (double newSampleRate, int maxBlockSize) override
    {
        incrFactor = juce::MathConstants<double>::twoPi / newSampleRate;
    }
    
    void process (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override
    {
        double increment = (double)freq * incrFactor;
        const float* triggerSamples = buffer.getReadPointer(0);
        const float* freqCVSamples = buffer.getReadPointer(1);
        const float* shapeCVSamples = buffer.getReadPointer(2);
        float* outSamples = buffer.getWritePointer(0);
        float* rampSamples = buffer.getWritePointer(1);
        
        if (triggerParameterWasOn()) phase = 0.0;
        
        // for now, we constantly process audio, but ideally, the module should know if it is connected
        for (int n = 0; n < buffer.getNumSamples(); n++)
        {
            const float trigger = *triggerSamples++;
            if ((previousTrigger - trigger) > 0.5f) phase = 0.0;
            
            double nextPhase = phase + increment * (double)pow(5.0f, *freqCVSamples++);
            
            *outSamples++ = processImpulse(phase, clip(shape + *shapeCVSamples++, 0.0f, 1.0f));
            *rampSamples++ = std::min(1.0f, (float)phase * invTwoPi);

            phase = nextPhase;
            phase = std::min(std::numeric_limits<double>::max(), phase);
            previousTrigger = trigger;
        }
    }
    
    void parameterChanged (const juce::String& parameterID, float value) override {
        if (parameterID == "freq") freq = value;
        else if (parameterID == "shape") shape = pow(value * 0.01f, 0.2f);
    }

    std::unique_ptr<ModuleUI> createUI() override { return std::make_unique<ImpulseUI>(*this); }
    
private:
    static constexpr float invTwoPi = 1.0f/juce::MathConstants<float>::twoPi;
    
    double incrFactor = 1.0, phase = 0.0;
    float previousTrigger = 0.0f;
    float freq = 20.0f, shape = 0.0f;
    
    bool triggerParameterWasOn()
    {
        return params.getRawParameterValue("trigger")->exchange(0.0f) > 0.0f;
    }
};
