/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"
//
//Font LookAndFeel_V4::getTextButtonFont(TextButton &, int buttonHeight)
//{
//    return { jmin (16.0f, buttonHeight * 0.6f) };
//}

//==============================================================================
MainComponent::MainComponent() :
patchCordTypeButton{"Patch Cord"}
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
    
    // Set the colors of PopupMenu
    LookAndFeel::getDefaultLookAndFeel().setColour(PopupMenu::textColourId, Colours::lightgrey);
    LookAndFeel::getDefaultLookAndFeel().setColour(PopupMenu::backgroundColourId, Colours::darkgrey.darker());
    LookAndFeel::getDefaultLookAndFeel().setColour(PopupMenu::highlightedBackgroundColourId, Colour::greyLevel(0.2));
    
    LookAndFeel::getDefaultLookAndFeel().setColour(TextButton::buttonColourId , Colours::transparentBlack);
    
    // Set the colors of Scrollbars
    LookAndFeel::getDefaultLookAndFeel().setColour(ScrollBar::thumbColourId, Colours::lightgrey.withAlpha(0.5f));
    
    LookAndFeel::getDefaultLookAndFeel().setDefaultSansSerifTypefaceName ("Helvetica Neue");
    
    sendLookAndFeelChange();
    
    setSize (600, 400);
    
    addChildComponent(mainPatcher);
    addAndMakeVisible(viewport);
    viewport.setViewedComponent(&mainPatcher, false);
    
    addAndMakeVisible(patchCordTypeButton);
    
    patchCordTypeButton.addListener(this);

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
    mainPatcher.mainProcessor->setPlayConfigDetails (mainPatcher.mainProcessor->getMainBusNumInputChannels(), mainPatcher.mainProcessor->getMainBusNumOutputChannels(), sampleRate, samplesPerBlockExpected);
 
    mainPatcher.mainProcessor->prepareToPlay (sampleRate, samplesPerBlockExpected);
}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
        for (int i = mainPatcher.mainProcessor->getTotalNumInputChannels(); i < mainPatcher.mainProcessor->getTotalNumOutputChannels(); ++i)
            bufferToFill.buffer->clear (i, 0, bufferToFill.buffer->getNumSamples());
    
    mainPatcher.mainProcessor->processBlock (*bufferToFill.buffer, dummyMidiBuffer);
    
//    bufferToFill.clearActiveBufferRegion();
}

void MainComponent::releaseResources()
{
    mainPatcher.mainProcessor->releaseResources();
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    g.fillAll (Colours::darkgrey);
    g.setColour(Colours::darkgrey.darker());
    g.fillRect(topBarBounds);
}

void MainComponent::resized()
{
    Rectangle<int> windowBounds = getLocalBounds();
    topBarBounds = windowBounds.removeFromTop(50);
    
    patchCordTypeButton.setBounds(topBarBounds.withTrimmedLeft(getWidth()-100).reduced(10));
    
    viewport.setBounds(windowBounds);
    mainPatcher.setSize(windowBounds.getWidth()*3, windowBounds.getHeight()*3);
}



void MainComponent::buttonClicked (Button* button)
{
    if (button == &patchCordTypeButton)
    {
        mainPatcher.togglePatchCordType();
    }
    
}
