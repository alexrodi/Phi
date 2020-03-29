/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "MainPatcher.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent   : public AudioAppComponent,
                        public DragAndDropContainer,
                        public Button::Listener

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

//==============================================================================
private:
    // Our LookAndFeel instance for the MainComponent
    // this maintains certain colors in the window but any module can surpass it with its own
    //LookAndFeel_V4 lookandfeel;
    
    // The Main Patcher where all modules and connections reside
    MainPatcher mainPatcher;
    
    // The window scrollbars
    Viewport viewport;
    
    TextButton patchCordTypeButton;
    
    Rectangle<int> topBarBounds;
    
    void buttonClicked (Button*) override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
