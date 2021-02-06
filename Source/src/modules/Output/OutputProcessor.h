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
#include "../../ModuleProcessor.h"

//==============================================================================
/*
*/
class OutputProcessor    : public ModuleProcessor
{
public:
    OutputProcessor();
    
    ~OutputProcessor();
    
    void prepareToPlay (double, int) override;
    void processBlock (AudioBuffer<float>& buffer, MidiBuffer&) override;
    void releaseResources() override;
    
    AudioProcessorEditor* createEditor() override;

private:
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OutputProcessor)
};
