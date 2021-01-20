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
#include "../../ModuleUI.h"

//==============================================================================
/*
*/
class GainProcessor    : public ModuleProcessor
{
public:
    GainProcessor();
    
    ~GainProcessor();
    
    void prepareToPlay (double, int) override;
    void processBlock (AudioBuffer<float>& buffer, MidiBuffer&) override;
    void releaseResources() override;
    
    std::unique_ptr<ModuleUI> createUI();

private:
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GainProcessor)
};
