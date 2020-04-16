/*
  ==============================================================================

    Module.h
    Created: 29 Mar 2020 11:38:39am
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

///@cond
#include <JuceHeader.h>
///@endcond
#include "Plug.h"

//==============================================================================
/*
 Module - The base class for all modules
*/
class Module    : public Component,
                  public AudioProcessor
{
public:
    
    struct Props
    {
        const String name;
        const Array<String> inlets;
        const Array<String> outlets;
        const int width;
        const int height;
        const int minimumHeight;
    };
    
    Module(Props);
    
    Props props;
    
    int inletNumber;
    int outletNumber;
    
    OwnedArray<Inlet> inlets;
    OwnedArray<Outlet> outlets;
    
    AudioProcessorGraph::NodeID nodeID;
    
    const Rectangle<int> placeInletsOutlets (Rectangle<int>);
    
    /**
     The base-class overrides resized()
     and exposes wasResized() containing
     the actual module's rectangle
    */
    void resized() override;
    virtual void wasResized(Rectangle<int>) {};
    
private:
    
    ///@cond
    const String getName() const override {return String("");};
    double getTailLengthSeconds() const override {return 0.0f;};
    bool acceptsMidi() const override {return false;};
    bool producesMidi() const override {return false;};
    AudioProcessorEditor* createEditor() override {return nullptr;};
    bool hasEditor() const override {return false;};
    int getNumPrograms() override {return 0;};
    int getCurrentProgram() override {return 0;};
    void setCurrentProgram (int index) override {};
    const String getProgramName (int index) override {return String("");};
    void changeProgramName (int index, const String& newName) override {};
    void getStateInformation (juce::MemoryBlock& destData) override {};
    void setStateInformation (const void* data, int sizeInBytes) override {};
    ///@endcond
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Module)
};
