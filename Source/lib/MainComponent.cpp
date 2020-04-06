/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent() :
patchCordTypeButton{"Patch Cord"}
{
    
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
