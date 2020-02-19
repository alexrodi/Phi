/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ModuleBox.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent   : public AudioAppComponent
{
//==============================================================================
public:
    
    MainComponent();
    ~MainComponent();

    // Audio =======================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (Graphics& g) override;
    void resized() override;
    
    void mouseDown(const MouseEvent& e) override;

//==============================================================================
private:

    OwnedArray<ModuleBox> modules;
    SelectedItemSet<ModuleBox*> selectedModules;
    LookAndFeel_V4 lookandfeel;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
