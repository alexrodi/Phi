/*
  ==============================================================================

    StringProcessor.h
    Created: 25 Mar 2020 3:14:32pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

///@cond
#include <JuceHeader.h>
///@endcond
#include "../../ModuleProcessor.h"
#include "../../DSPUtils.h"

//==============================================================================
/*
*/
class StringProcessor    : public ModuleProcessor
{
public:
    StringProcessor();
    
    ~StringProcessor();
    
    void prepareToPlay (double, int) override;
    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;
    void releaseResources() override;
    
    AudioProcessorEditor* createEditor() override;

private:
    
    float sampleRate = 44100.0f;
    
    DelayLine<float> line1, line2;
    OnePole<float> onePole1, onePole2;
    DCBlock<float> dcBlock1, dcBlock2;
    Accum<float> accum1, accum2;
    
    const float processLine1Node(float, float, float, bool);
    const float processLine2Node(float, float, float);
    const float readOutput1(float);
    const float readOutput2(float);
    const float scaleDecay(float, bool);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StringProcessor)
};
