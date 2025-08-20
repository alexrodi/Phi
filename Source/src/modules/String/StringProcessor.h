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
struct StringProcessor : ModuleProcessor
{
    StringProcessor() :
    ModuleProcessor(5, 1,
        std::make_unique<FloatParameter> (
            "freq",
            "Frequency",
            NormalisableRange<float>(20.0f, 10000.0f, 0.01, 0.3f),
            220.0f,
            FloatParameter::Attributes{}.withLabel("Hz")
        ),
        std::make_unique<FloatParameter> (
            "damp",
            "Damping",
            NormalisableRange<float>(0.0f, 100.0f),
            0.0f,
            FloatParameter::Attributes{}.withLabel("%")
        ),
        std::make_unique<FloatParameter> (
            "pos",
            "Position",
            NormalisableRange<float>(0.0f, 100.0f),
            25.0f,
            FloatParameter::Attributes{}.withLabel("%")
        ),
        std::make_unique<FloatParameter> (
            "decay",
            "Decay",
            NormalisableRange<float>(0.0f, 100.0f),
            90.0f,
            FloatParameter::Attributes{}.withLabel("%")
        ),
        std::make_unique<AudioParameterBool>  (
            "mode" ,
            "Mode"     ,
            false
        )
    )
    {}
    
    ~StringProcessor() {}
    
    void prepare (double newSampleRate, int maxBlockSize) override
    {
        sampleRate = newSampleRate;
        line1.resize(newSampleRate);
        line2.resize(newSampleRate);
        
        line1.clear();
        line2.clear();
        
        onePole1.prepare(newSampleRate);
        onePole2.prepare(newSampleRate);
        
        dcBlock.prepare(newSampleRate);
        accum.reset();
       
    }
    
    void process (AudioBuffer<float>& buffer, MidiBuffer&) override
    {
        const float* inputSamples = buffer.getReadPointer(0);
        const float* freqCVSamples = buffer.getReadPointer(1);
        const float* posCVSamples = buffer.getReadPointer(2);
        const float* dampCVSamples = buffer.getReadPointer(3);
        const float* decayCVSamples = buffer.getReadPointer(4);
        
        float* outputSamples = buffer.getWritePointer(0);
        
        // Mode B doubles the (perceived) interval so we must divide it accordingly
        float modeFactor = mode == Mode::B ? 0.5f : 1.0f;
        
        for (int n = 0; n < buffer.getNumSamples(); n++)
        {
            float periodInSamples = sampleRate / std::min(frequency * pow(5.0f, *freqCVSamples++), 10000.0f);
            
            float scaledDecay = scaleDecay(clip(decay + *decayCVSamples++, 0.0f, 1.0f), mode);
            float feedback = getFeedback(periodInSamples, scaledDecay);
            float dampHz = pow(1.0f - clip(damp + *dampCVSamples++, 0.0f, 1.0f), 3.0f) * 20000.0f;
            
            // The interval to apply to each delay line
            float interval = periodInSamples * modeFactor;
            
            // Pickup position is always a fraction of the interval
            float line1Pos = interval * clip(pos + *posCVSamples++, 0.0f, 1.0f);
            float line2Pos = interval - line1Pos;
            
            float input = (*inputSamples++) * 0.05f;
            
            line1.push(   input + processLine1Node(dampHz, feedback, interval, mode) );
            line2.push( - input + processLine2Node(dampHz, feedback, interval) );
            
            *outputSamples++ = readOutput(line1Pos, line2Pos);
        }
    }
    
    void parameterChanged (const String& parameterID, float value) override {
        if (parameterID == "mode") mode = (Mode)value;
        else if (parameterID == "damp") damp = value * 0.01f;
        else if (parameterID == "freq") frequency = value;
        else if (parameterID == "decay") decay = value * 0.01f;
        else if (parameterID == "pos") pos = value * 0.01f;
    }
    
    AudioProcessorEditor* createEditor() override {return new StringUI(*this);}

private:
    float sampleRate = 44100.0f;
    
    DelayLine<float> line1, line2;
    OnePole<float> onePole1, onePole2;
    DCBlock<float> dcBlock;
    Accum<float> accum;
    
    enum class Mode {A, B} mode = Mode::A;
    float damp = 0.0f, frequency = 20.0f, decay = 0.0f, pos = 0.0f;
    
    float processLine1Node(float dampHz, float feedback, float interval, Mode mode)
    {
        onePole1.setCutoff(dampHz);
        
        // Line 1 gets a "special" function to liven the sound up a bit...
        float line1Node = sinf( onePole1.process( line1.get( interval ) ) * MathConstants<float>::twoPi * 1.5f );
        
        // Apply decay
        line1Node *= feedback * 0.1063f;
        
        // For our B mode, inverting the node changes the sound
        if ( mode == Mode::B ) line1Node *= -1.00005f;
        
        return line1Node;
    }
    
    float processLine2Node(float dampHz, float feedback, float interval)
    {
        onePole2.setCutoff(dampHz);
        
        // Normal case for Line 2
        // read from delay line, apply low pass and feedback (decay)
        float line2Node = onePole2.process( line2.get( interval ) ) * feedback;
        return line2Node;
    }
    
    float readOutput(float line1Pos, float line2Pos)
    {
        // Read from the delay line with the current position, integrate and highpass (dcblock)
        return dcBlock.process( accum.process( line1.get( line1Pos ) + line2.get( line2Pos )));
    }
    
    constexpr float scaleDecay(float decay, Mode mode)
    {
        return pow(decay, mode == Mode::B ? 0.0005f : 0.05f);
    }
    
    constexpr float getFeedback(float periodInSamples, float decay) {
        float exponent = periodInSamples * (0.01f + (mode == Mode::B ? 0.52f : 0.0f));
        return pow(decay, exponent) * 0.997f;
    }
};

