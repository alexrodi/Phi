
#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent(State& state) :
state(state),
patcher(state),
patchCordTypeButton("Gravity"),
showPortLabelsButton("Hint", "Label")
{
    juce::LookAndFeel::setDefaultLookAndFeel(&phiLookAndFeel);
    
    setSize (1000, 600);
    
    addChildComponent(patcher);
    addAndMakeVisible(viewport);
    viewport.setViewedComponent(&patcher, false);
    
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
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    g.fillAll(findColour(PhiColourIds::General::Background));
    
    g.setColour(findColour(PhiColourIds::General::TopBar));
    g.fillRect(topBarBounds);
}

void MainComponent::resized()
{
    auto bounds = getLocalBounds();
    
    topBarBounds = bounds.removeFromTop(50);
    
    auto buttonBounds = topBarBounds;
    
    patchCordTypeButton.setBounds(buttonBounds.removeFromRight(100).reduced(10));
    showPortLabelsButton.setBounds(buttonBounds.removeFromRight(150).reduced(10));
    
    viewport.setBounds(bounds);
    patcher.setSize(bounds.getWidth(), bounds.getHeight());
}
