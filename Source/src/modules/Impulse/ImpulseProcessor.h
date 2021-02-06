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
#include "../../ModuleProcessor.h"

class ImpulseProcessor : public ModuleProcessor
{
public:
    ImpulseProcessor();
    
    ~ImpulseProcessor();
    
    void prepareToPlay (double, int) override;
    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;
    void releaseResources() override;
    
    AudioProcessorEditor* createEditor() override;
    
    void triggerImpulse();
    
    // Definition is public for the UI to build a faithful representation
    inline static float processImpulse(float phase, float shape)
    {
        const float fundamentalAttenuator = (-0.5*tanhf(phase*(-fmax(shape,0.88)+1.01)-1)+0.5);

        return (phase==float_Pi)
               ? 0.0f
               : sinf((sinf(phase))/((-shape + 1.006f)*(phase-float_Pi)))*fundamentalAttenuator;
    }
    
private:

    float currentPhase = 0.0f;
    float previousTrigger = 0.0f;
    float sampleRate = 44100.0f;
    Atomic<bool> triggered;
    
    bool externallyTriggered();
};



