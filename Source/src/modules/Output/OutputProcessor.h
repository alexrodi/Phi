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
class OutputProcessor    : public ModuleProcessor
{
public:
    OutputProcessor() :
    ModuleProcessor(2, 2) // Output cheats on the number of output channels, the two outs are only used to patch it to the actual output device
    {
        isOutput = true;
    }
    
    ~OutputProcessor() {};
    
    void prepareToPlay (double sampleRate, int maxBlockSize) override {}
    void processBlock (AudioBuffer<float>& buffer, MidiBuffer&) override {}
    void releaseResources() override {}
    
    AudioProcessorEditor* createEditor() override {return new OutputUI(*this);}

private:
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OutputProcessor)
};
