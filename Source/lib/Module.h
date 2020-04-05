/*
  ==============================================================================

    Module.h
    Created: 29 Mar 2020 11:38:39am
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "phi_Inlet.h"
#include "phi_Outlet.h"

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
        const int inletNumber;
        const int outletNumber;
        const int width;
        const int height;
        const int minimumHeight;
    };
    
    Module(Props);
    
    OwnedArray<phi_Inlet> inlets;
    OwnedArray<phi_Outlet> outlets;
    
    Props props;
    
    const Rectangle<int> placeInletsOutlets (Rectangle<int>);
    
private:
    
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
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Module)
};
