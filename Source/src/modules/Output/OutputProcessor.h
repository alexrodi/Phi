/*
  ==============================================================================

    OutputProcessor.h
    Created: 25 Apr 2020 12:10:44am
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

#include "OutputUI.h"

//==============================================================================
/*
*/
struct OutputProcessor : ModuleProcessor
{
    OutputProcessor() :
    ModuleProcessor(2/* Inlets */, 2/* Outlets */)
    {
        isOutput = true;
    }
    
    ~OutputProcessor() {};
    
    void prepare (double sampleRate, int maxBlockSize) override {}
    void process (juce::AudioBuffer<float>& buffer, juce::MidiBuffer&) override {}
    
    std::unique_ptr<ModuleUI> createUI() override { return std::make_unique<OutputUI>(*this); }
};
