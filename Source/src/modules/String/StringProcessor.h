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
#include "StringUI.h"
#include "../../DSPUtils.h"

//==============================================================================
/*
*/
class StringProcessor    : public ModuleProcessor
{
public:
    StringProcessor() :
    ModuleProcessor(1, 1,
        std::make_unique<AudioParameterFloat> (
            "freq",
            "Frequency",
            NormalisableRange<float>(20.0f, 10000.0f, 0.0f, 0.3f),
            220.0f,
            "Frequency",
            AudioParameterFloat::genericParameter,
            [](float value, int) { return String (value, 1); },
            [](const String& text) { return text.getFloatValue(); }
        ),
        std::make_unique<AudioParameterFloat> (
            "damp",
            "Damping",
            NormalisableRange<float>(0.0f, 1.0f),
            0.0f,
            "Damping",
            AudioParameterFloat::genericParameter,
            [](float value, int) { return String (floorf(value * 100.0f), 0); },
            [](const String& text) { return text.getFloatValue() * 0.01f; }
        ),
        std::make_unique<AudioParameterFloat> (
            "pos",
            "Position",
            NormalisableRange<float>(0.0f, 1.0f),
            0.25f,
            "Position",
            AudioParameterFloat::genericParameter,
            [](float value, int) { return String (floorf(value * 100.0f), 0); },
            [](const String& text) { return text.getFloatValue() * 0.01f; }
        ),
        std::make_unique<AudioParameterFloat> (
            "decay",
            "Decay",
            NormalisableRange<float>(0.0f, 1.0f),
            0.9f,
            "Decay",
            AudioParameterFloat::genericParameter,
            [](float value, int) { return String (floorf(value * 100.0f), 0); },
            [](const String& text) { return text.getFloatValue() * 0.01f; }
        ),
        std::make_unique<AudioParameterBool>  (
            "mode" ,
            "Mode"     ,
            false
        )
    )
    {}
    
    ~StringProcessor() {}
    
    void prepareToPlay (double newSampleRate, int maxBlockSize) override
    {
        sampleRate = newSampleRate;
        line1.resize(newSampleRate);
        line2.resize(newSampleRate);
        
        line1.clear();
        line2.clear();
        
        onePole1.reset();
        onePole2.reset();
        
        dcBlock.reset();
        accum.reset();
       
    }
    
    void processBlock (AudioBuffer<float>& buffer, MidiBuffer&) override
    {
        const float* inputSamples = buffer.getReadPointer(0);
        
        float* outputSamples = buffer.getWritePointer(0);
        
        const float periodInSamples = sampleRate / frequency;
        
        float feedback = getFeedback(periodInSamples);
         
        // The interval to apply to each delay line
        // Mode B doubles the (perceived) interval so we must divide it accordingly
        const float interval = periodInSamples * (mode == Mode::B ? 0.5f : 1.0f);
        
        // Pickup position is always a fraction of the interval
        const float line1Pos = interval * pos;
        const float line2Pos = interval - line1Pos;
        
        for (int n = 0; n < buffer.getNumSamples(); n++)
        {
            float input = (*inputSamples++) * 0.05f;
            
            line1.push(   input + processLine1Node(damp, feedback, interval, mode) );
            line2.push( - input + processLine2Node(damp, feedback, interval) );
            
            *outputSamples++ = readOutput(line1Pos, line2Pos);
        }
    }
    
    void parameterChanged (const String& parameterID, float value) override {
        if (parameterID == "mode") {
            mode = (Mode)value;
            decay = scaleDecay(value, mode);
        } else if (parameterID == "damp") damp = value;
        else if (parameterID == "freq") frequency = value;
        else if (parameterID == "decay") decay = scaleDecay(value, mode);
        else if (parameterID == "pos") pos = value;
    }
    
    AudioProcessorEditor* createEditor() override {return new StringUI(*this);}

private:
    float sampleRate = 44100.0f;
    
    DelayLine<float> line1, line2;
    OnePole<float> onePole1, onePole2;
    DCBlock<float> dcBlock;
    Accum<float> accum;
    
    enum class Mode {A, B} mode = Mode::A;
    float damp = 0.0f, frequency = 1.0f, decay = 0.0f, pos = 0.0f;
    
    float processLine1Node(float damp, float feedback, float interval, Mode mode)
    {
        // Line 1 gets a "special" function to liven the sound up a bit...
        float line1Node = sinf( onePole1.process( line1.get( interval ), damp ) * MathConstants<float>::twoPi * 1.5f );
        
        // Apply decay
        line1Node *= feedback * 0.1063f;
        
        // For our B mode, inverting the node changes the sound
        if ( mode == Mode::B ) line1Node *= -1.00005f;
        
        return line1Node;
    }
    
    float processLine2Node(float damp, float feedback, float interval)
    {
        // Normal case for Line 2
        // read from delay line, apply low pass and feedback (decay)
        float line2Node = onePole2.process( line2.get( interval ), damp ) * feedback;
        return line2Node;
    }
    
    float readOutput(float line1Pos, float line2Pos)
    {
        // Read from the delay line with the current position, integrate and highpass (dcblock)
        return dcBlock.process( accum.process( line1.get( line1Pos ) + line2.get( line2Pos )));
    }
    
    constexpr float scaleDecay(float decay, Mode mode)
    {
        return powf(decay, mode == Mode::B ? 0.0005f : 0.05f);
    }
    
    constexpr float getFeedback(float periodInSamples) {
        float exponent = periodInSamples * (0.01f + (mode == Mode::B ? 0.52f : 0.0f));
        return pow(decay, exponent) * 0.997f;
    }
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StringProcessor)
};

