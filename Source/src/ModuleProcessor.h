/*
  ==============================================================================

    ModuleProcessor.h
    Created: 29 Mar 2020 11:38:39am
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

///@cond
#include <JuceHeader.h>
///@endcond
#include "ModuleUI.h"

//==============================================================================
/**
 The base class for all modules' DSP implementation
*/
class ModuleProcessor    : public AudioProcessor
{
public:
    /// Any parameters introduced in the constructor will be stored in this object
    AudioProcessorValueTreeState params;
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
    params( *this, nullptr, "PARAMETERS", AudioProcessorValueTreeState::ParameterLayout(std::move(paramsToUse)...) )
    {
        setPlayConfigDetails (inletNumber, outletNumber, getSampleRate(), getBlockSize());
    }
    
    std::unique_ptr<ModuleUI> createUI() {
        return std::unique_ptr<ModuleUI>(static_cast<ModuleUI*>(createEditor()));
    }
    
    bool hasEditor() const override { return true; }
    
private:
    
    ///@cond
    const String getName() const override {return String("");}
    double getTailLengthSeconds() const override {return 0.0f;}
    bool acceptsMidi() const override {return false;}
    bool producesMidi() const override {return false;}
    void releaseResources() override {}
    int getNumPrograms() override {return 0;}
    int getCurrentProgram() override {return 0;}
    void setCurrentProgram (int index) override {}
    const String getProgramName (int index) override {return String("");}
    void changeProgramName (int index, const String& newName) override {}
    void getStateInformation (MemoryBlock& destData) override {}
    void setStateInformation (const void* data, int sizeInBytes) override {}
    ///@endcond
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ModuleProcessor)
};
