
#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent(State& state) :
state(state),
mainPatcher(state),
patchCordTypeButton("Gravity"),
showPortLabelsButton("Hint", "Label")
{
    setLookAndFeel (&phiLookAndFeel);
    
    setSize (1000, 600);
    
    addChildComponent(mainPatcher);
    addAndMakeVisible(viewport);
    viewport.setViewedComponent(&mainPatcher, false);
    
    addAndMakeVisible(patchCordTypeButton);
    addAndMakeVisible(showPortLabelsButton);
    
    patchCordTypeButton.onClick = [&] () {
        state.setPatchCordType((PatchCordType)patchCordTypeButton.getToggleState());
    };
    
    showPortLabelsButton.onClick = [&] () {
        state.setShowPortLabels((ShowPortLabels)showPortLabelsButton.getToggleState());
    };
    
    sendLookAndFeelChange();
    
    setPaintingIsUnclipped(true);
}

MainComponent::~MainComponent()
{
    setLookAndFeel(nullptr);
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::darkgrey);
    g.setColour(juce::Colours::darkgrey.darker());
    g.fillRect(topBarBounds);
}

void MainComponent::resized()
{
    juce::Rectangle<int> windowBounds(getLocalBounds());
    topBarBounds = windowBounds.removeFromTop(50);
    
    juce::Rectangle<int> buttonBounds(topBarBounds);
    
    patchCordTypeButton.setBounds(buttonBounds.removeFromRight(100).reduced(10));
    showPortLabelsButton.setBounds(buttonBounds.removeFromRight(150).reduced(10));
    
    viewport.setBounds(windowBounds);
    mainPatcher.setSize(windowBounds.getWidth()*3, windowBounds.getHeight()*3);
}
