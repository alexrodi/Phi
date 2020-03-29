/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Some platforms require permissions to open input channels so request that here
    if (RuntimePermissions::isRequired (RuntimePermissions::recordAudio)
        && ! RuntimePermissions::isGranted (RuntimePermissions::recordAudio))
    {
        RuntimePermissions::request (RuntimePermissions::recordAudio,
                                     [&] (bool granted) { if (granted)  setAudioChannels (2, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }
    
    setLookAndFeel(&lookandfeel);
    
    // Set the colors of PopupMenu
    lookandfeel.setColour(PopupMenu::textColourId, Colours::lightgrey);
    lookandfeel.setColour(PopupMenu::backgroundColourId, Colours::darkgrey.darker());
    lookandfeel.setColour(PopupMenu::highlightedBackgroundColourId, Colour::greyLevel(0.2));
    
    // Set the colors of Scrollbars
    lookandfeel.setColour(ScrollBar::thumbColourId, Colours::lightgrey.withAlpha(0.5f));
    
    sendLookAndFeelChange();
    
    // Apply our colors to the menu
    //rightClickMenu.setLookAndFeel(&lookandfeel);
    
    setSize (600, 400);
    
    addChildComponent(mainPatcher);
    addAndMakeVisible(viewport);
    viewport.setViewedComponent(&mainPatcher, false);
}

MainComponent::~MainComponent()
{
    setLookAndFeel(nullptr);
    
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    // For more details, see the help for AudioProcessor::prepareToPlay()
}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    // Your audio-processing code goes here!

    // For more details, see the help for AudioProcessor::getNextAudioBlock()

    // Right now we are not producing any data, in which case we need to clear the buffer
    // (to prevent the output of random noise)
    bufferToFill.clearActiveBufferRegion();
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    g.fillAll (Colours::darkgrey);
}

void MainComponent::resized()
{
    viewport.setSize(getWidth(), getHeight());
    mainPatcher.setSize(getWidth()*3, getHeight()*3);
}
