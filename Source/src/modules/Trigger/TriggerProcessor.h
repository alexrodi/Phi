/*
  ==============================================================================

    GainProcessor.h
    Created: 21 Apr 2020 8:18:03pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

///@cond
#include <JuceHeader.h>
///@endcond
#include "TriggerUI.h"

//==============================================================================
/*
*/
class TriggerProcessor    : public ModuleProcessor
{
public:
    TriggerProcessor() :
    ModuleProcessor(
        1, // Inlets
        1, // Outlets
        //============= Parameters =============
        std::make_unique<AudioParameterBool> (
            "trigger",
            "Trigger",
            0.0f
        )
    )
    {}
    
    ~TriggerProcessor() {}
    
    void prepareToPlay (double sampleRate, int maxBlockSize) override {}
    
    void processBlock (AudioBuffer<float>& buffer, MidiBuffer&) override
    {
        float triggered = *params.getRawParameterValue("trigger");
        float* samples = buffer.getWritePointer(0);
        
        for (auto i=0; i<buffer.getNumSamples(); i++)
            samples[i] = triggered;
    }
    
    void releaseResources() override {}
    
    AudioProcessorEditor* createEditor() override {return new TriggerUI(*this);}

private:
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TriggerProcessor)
};
