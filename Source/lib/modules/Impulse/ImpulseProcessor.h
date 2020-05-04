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
#include "../../ModuleUI.h"

class ImpulseProcessor : public ModuleProcessor
{
public:
    ImpulseProcessor();
    
    ~ImpulseProcessor();
    
    void prepareToPlay (double, int) override;
    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;
    void releaseResources() override;
    
    std::unique_ptr<ModuleUI> createUI();
    
    void triggerImpulse();
    
    inline static float processImpulse(float phase, float shape)
    {
        const float fundamentalAttenuator = (-0.5*tanhf(phase*(-fmax(shape,0.88)+1.01)-1)+0.5);

        return (phase==float_Pi)
               ? 0.f
               : sin((sin(phase))/((-shape + 1.006)*(phase-float_Pi)))*fundamentalAttenuator;
    }
    
private:

    float currentPhase = 0.0f;
    float currentTrigger = 0.0f;
    float sampleRate = 44100.0f;
};



