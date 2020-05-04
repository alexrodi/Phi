/*
  ==============================================================================

    ImpusleProcessor.cpp
    Created: 25 Apr 2020 12:10:22am
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#include "ImpulseProcessor.h"
#include "ImpulseUI.h"

const float invTwoPi = 1.0f/MathConstants<float>::twoPi;

ImpulseProcessor::ImpulseProcessor() :
ModuleProcessor( 3, 2
                     , std::make_unique<AudioParameterFloat> ("freq", "Frequency", NormalisableRange<float> (20.0f, 20000.0f), 20.0f)
                     , std::make_unique<AudioParameterFloat> ("shape", "Shape", NormalisableRange<float> (0.0f, 1.0f), 0.0f)
)
{
}

ImpulseProcessor::~ImpulseProcessor()
{
}

void ImpulseProcessor::prepareToPlay (double newSampleRate, int maximumExpectedSamplesPerBlock)
{
    sampleRate = float(newSampleRate);
}

void ImpulseProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    const float* readBufferTrigger = buffer.getReadPointer(0);
    //const float* readBufferFreq = buffer.getReadPointer(1);
    //const float* readBufferShape = buffer.getReadPointer(2);
    
    const float shape = powf(*params.getRawParameterValue("shape"), 0.1f);
    
    const float phaseIncrement = (1.0f/(sampleRate/(*params.getRawParameterValue("freq")))) * MathConstants<float>::twoPi;
    
    float* writeBufferOut = buffer.getWritePointer(0);
    float* writeBufferRamp = buffer.getWritePointer(1);
    
    // for now, we constantly process audio, but ideally, the module should know if it is connected
    for (int n = 0; n < buffer.getNumSamples(); n++)
    {
        const float trigger = *readBufferTrigger++;
        const float triggerDelta = currentTrigger - trigger;
        currentTrigger = trigger;
        
        if (triggerDelta > 0.5f) triggerImpulse();
        
        *writeBufferOut++ = processImpulse(currentPhase, shape);
        *writeBufferRamp++ = currentPhase * invTwoPi;

        currentPhase += phaseIncrement;
    }
    
}

void ImpulseProcessor::releaseResources()
{
}

void ImpulseProcessor::triggerImpulse()
{
    currentPhase = 0;
}

std::unique_ptr<ModuleUI> ImpulseProcessor::createUI()
{
   return std::make_unique<ImpulseUI>(*this);
}
