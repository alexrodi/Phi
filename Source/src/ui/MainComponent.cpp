
#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent(State& state) :
state(state),
mainPatcher(state),
patchCordTypeButton("Gravity"),
inoutNamesTypeButton("Hint / Label")
{
    setLookAndFeel (&phiLookAndFeel);
    
    setSize (1000, 600);
    
    addChildComponent(mainPatcher);
    addAndMakeVisible(viewport);
    viewport.setViewedComponent(&mainPatcher, false);
    
    addAndMakeVisible(patchCordTypeButton);
    addAndMakeVisible(inoutNamesTypeButton);
    
    patchCordTypeButton.addListener(this);
    inoutNamesTypeButton.addListener(this);
    
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
    inoutNamesTypeButton.setBounds(buttonBounds.removeFromRight(100).reduced(10));
    
    viewport.setBounds(windowBounds);
    mainPatcher.setSize(windowBounds.getWidth()*3, windowBounds.getHeight()*3);
}

void MainComponent::buttonClicked (juce::Button* button)
{
    if (button == &patchCordTypeButton)
        state.setPatchCordType((PatchCordType)button->getToggleState());
    else if (button == &inoutNamesTypeButton)
        state.setShowPortLabels((ShowPortLabels)button->getToggleState());
}
