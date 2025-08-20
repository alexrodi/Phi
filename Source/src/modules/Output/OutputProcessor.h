/*
  ==============================================================================

    OutputProcessor.h
    Created: 25 Apr 2020 12:10:44am
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

///@cond
#include <JuceHeader.h>
///@endcond
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
    void process (AudioBuffer<float>& buffer, MidiBuffer&) override {}
    
    AudioProcessorEditor* createEditor() override {return new OutputUI(*this);}
};
