/*
  ==============================================================================

    FilterProcessor.h
    Created: 21 Aug 2025 12:24:03am
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

#include "FilterUI.h"

//==============================================================================
/*
*/
struct FilterProcessor : ModuleProcessor
{
    FilterProcessor() :
    ModuleProcessor(
        3, // Inlets
        3, // Outlets
        //============= Parameters =============
        std::make_unique<FloatParameter> (
            "freq",
            "Freq",
            juce::NormalisableRange<float> (20.0f, 20000.0f, 0.0, 0.2f),
            1000.0f,
            FloatParameter::Attributes{}.withLabel("Hz")
        ),
        std::make_unique<FloatParameter> (
            "res",
            "Res",
            juce::NormalisableRange<float> (0.0f, 100.0f),
            25.0f,
            FloatParameter::Attributes{}.withLabel("%")
        )
    )
    {}
    
    ~FilterProcessor() {}
    
    void prepare (double newSampleRate, int maxBlockSize) override {
        filter.prepare(newSampleRate);
    }
    
    void process (juce::AudioBuffer<float>& buffer, juce::MidiBuffer&) override
    {
        filter.process(buffer, buffer.getReadPointer(1), buffer.getReadPointer(2));
    }
    
    void parameterChanged (const juce::String& parameterID, float value) override {
        if (parameterID == "freq") filter.setFrequency(value);
        else if (parameterID == "res") filter.setResonance(value * 0.01f);
    }
    
    std::unique_ptr<ModuleUI> createUI() override { return std::make_unique<FilterUI>(*this); }
    
private:
    StateVariableFilter filter;
};
