/*
  ==============================================================================

    ModuleProcessor.h
    Created: 29 Mar 2020 11:38:39am
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

#include "DSPUtils.h"
#include "ui/ModuleUI.h"

//==============================================================================
struct FloatParameter : juce::AudioParameterFloat {
    using Attributes = juce::AudioParameterFloatAttributes;
    
    FloatParameter(const juce::ParameterID& parameterID,
                   const juce::String& parameterName,
                   juce::NormalisableRange<float> normalisableRange,
                   float defaultValue,
                   const Attributes& attributes = {}) :
    AudioParameterFloat(parameterID, parameterName, normalisableRange, defaultValue, [&] () {
        if (attributes.getStringFromValueFunction() == nullptr) {
            auto label = attributes.getAudioProcessorParameterWithIDAttributes().getLabel();
            
            return attributes.withStringFromValueFunction([label] (float value, int) {
                auto upTo2Decimals = [] (float value) {
                    return juce::String(value, (abs(value) < 10.0f) ? 2 : 1);
                };
                
                if (abs(value) < 100.0f)
                    return upTo2Decimals(value) + " " + label;
                else if (value > 1000.0f)
                    return upTo2Decimals(value * 0.001f) + " k" + label;
                else
                    return juce::String((int)value) + " " + label;
            });
        } else return attributes;
    }())
    {}
};

/**
 The base class for all modules' DSP implementation
*/
class ModuleProcessor    : public juce::AudioProcessor,
                           public juce::AudioProcessorValueTreeState::Listener
{
public:
    using Parameters = juce::AudioProcessorValueTreeState::ParameterLayout;
    
    /// Any parameters introduced in the constructor will be stored in this object
    juce::AudioProcessorValueTreeState params;
    /// Output modules must set this to true, they should still define the number of output channels but they won't be displayed in the UI
    bool isOutput = false;
    
    /**
     * Constructs a processor for a module
     * The third and any other following argument allows you to declare any parameters for this processor as parameter pack
     * @param inletNumber Number of inlets
     * @param outletNumber Number of outlets (in output modules, this number corresponds to the number of channels to connect to the audio output device)
     * @param paramsToUse Parameter pack of std::unique_ptr<RangedAudioParameter> to initialize the processor parameter tree
     @code
     MyModuleProcessor() :
     ModuleProcessor( 2, 2,
       std::make_unique<AudioParameterFloat> ("gain", "Gain", NormalisableRange<float> (0.0f, 1.0f), 1.0f),
       std::make_unique<AudioParameterFloat> ("freq", "Frequency", NormalisableRange<float> (20.0f, 20000.0f), 20.0f)
     )
     {...}
     @endcode
     */
    template <typename... Items>
    ModuleProcessor(int inletNumber, int outletNumber, std::unique_ptr<Items>... paramsToUse) :
    params( *this, nullptr, "PARAMETERS", juce::AudioProcessorValueTreeState::ParameterLayout(std::move(paramsToUse)...) )
    {
        setPlayConfigDetails (inletNumber, outletNumber, getSampleRate(), getBlockSize());
        
        // Add parameter listeners
        for (int i = 0; i < params.state.getNumChildren(); i++){
            auto paramID = params.state.getChild(i).getProperty("id").toString();
            params.addParameterListener(paramID, this);
            parameterChanged(paramID, *params.getRawParameterValue(paramID));
        }
    }
    
    virtual ~ModuleProcessor() = default;
    
    /// Creates the UI for this Module
    virtual std::unique_ptr<ModuleUI> createUI() = 0;
    
    /// Called before playback starts, to let the processor prepare itself.
    virtual void prepare (double newSampleRate, int maxBlockSize) = 0;
    
    /// Renders the next block.
    virtual void process (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) = 0;
    
    /// Override this to get notified of parameter changes
    virtual void parameterChanged (const juce::String& parameterID, float newValue) override {}
    
private:
    
    ///@cond
    const juce::String getName() const override {return "";}
    double getTailLengthSeconds() const override {return 0.0f;}
    bool acceptsMidi() const override {return false;}
    bool producesMidi() const override {return false;}
    void releaseResources() override {}
    int getNumPrograms() override {return 0;}
    int getCurrentProgram() override {return 0;}
    void setCurrentProgram (int index) override {}
    const juce::String getProgramName (int index) override {return "";}
    void changeProgramName (int index, const juce::String& newName) override {}
    void getStateInformation (juce::MemoryBlock& destData) override {}
    void setStateInformation (const void* data, int sizeInBytes) override {}
    juce::AudioProcessorEditor* createEditor() override { return nullptr; }
    bool hasEditor() const override { return false; }
    
    void prepareToPlay (double newSampleRate, int maxBlockSize) override
    {
        // Initialise parameters
        for (int i = 0; i < params.state.getNumChildren(); i++){
            auto paramID = params.state.getChild(i).getProperty("id").toString();
            parameterChanged(paramID, *params.getRawParameterValue(paramID));
        }
        
        prepare(newSampleRate, maxBlockSize);
    }
    
    void processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override
    {
        process(buffer, midiMessages);
    }
    ///@endcond
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ModuleProcessor)
};
