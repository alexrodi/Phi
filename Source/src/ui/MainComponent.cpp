
#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent(State& state) :
state(state),
patcher(state),
fileMenuModel(state),
fileMenu(&fileMenuModel),
patchCordTypeButton("Gravity"),
showPortLabelsButton("Hint", "Label")
{
    setPaintingIsUnclipped(true);
    
    addChildComponent(patcher);
    addAndMakeVisible(viewport);
    viewport.setViewedComponent(&patcher, false);
    
    addAndMakeVisible(fileMenu);
    addAndMakeVisible(patchCordTypeButton);
    addAndMakeVisible(showPortLabelsButton);
    
    patchCordTypeButton.onClick = [&] () {
        state.setPatchCordType((PatchCordType)patchCordTypeButton.getToggleState());
    };
    
    showPortLabelsButton.onClick = [&] () {
        state.setShowPortLabels((ShowPortLabels)showPortLabelsButton.getToggleState());
    };
    
    juce::LookAndFeel::setDefaultLookAndFeel(&phiLookAndFeel);
    
    // TODO: Components shouldn't reach out for module colours by default!
    // Components out here have a neutral highlight colour
    phiLookAndFeel.setColour(PhiColourIds::Module::Highlight, juce::Colour::greyLevel(0.8f));
    
    setSize (1000, 600);
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
    
    fileMenu.setBounds(buttonBounds.removeFromLeft(100).reduced(10));
    patchCordTypeButton.setBounds(buttonBounds.removeFromRight(100).reduced(10));
    showPortLabelsButton.setBounds(buttonBounds.removeFromRight(150).reduced(10));
    
    viewport.setBounds(bounds);
    patcher.setSize(bounds.getWidth(), bounds.getHeight());
}

void MainComponent::fileLoaded(juce::File file) {
    getTopLevelComponent()->setName("Phi [" + file.getFileNameWithoutExtension() + "]");
}
