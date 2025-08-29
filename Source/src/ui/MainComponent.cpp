
#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent(State& state, FileManager& fileManager) :
state(state),
patcher(state),
menuBar(*this, fileManager),
patchCordTypeButton("Gravity"),
showPortLabelsButton("Hint", "Label")
{
    // ======================= Look and Feel =======================
    // TODO: Components shouldn't reach for module colours!
    juce::LookAndFeel::setDefaultLookAndFeel(&lookandfeel);
    lookandfeel.setTheme({}, true);
    
    // ======================= Children =======================
    addAndMakeVisible(menuBar);
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
    
    // ======================= Listeners =======================
    state.addListener(this);
    
    // ======================= Settings =======================
    setPaintingIsUnclipped(true);
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
    g.fillRect(getLocalBounds().removeFromTop(topBarHeight));
}

void MainComponent::resized()
{
    auto bounds = getLocalBounds();
    auto topBarBounds = bounds.removeFromTop(topBarHeight);
    
    menuBar.setBounds(topBarBounds.removeFromLeft(100).reduced(10));
    patchCordTypeButton.setBounds(topBarBounds.removeFromRight(100).reduced(10));
    showPortLabelsButton.setBounds(topBarBounds.removeFromRight(150).reduced(10));
    
    viewport.setBounds(bounds);
    patcher.setSize(bounds.getWidth(), bounds.getHeight());
}

void MainComponent::fileLoaded(juce::File file) {
    getTopLevelComponent()->setName("Phi [" + file.getFileNameWithoutExtension() + "]");
}

void MainComponent::fileSaved(juce::File file) {
    getTopLevelComponent()->setName("Phi [" + file.getFileNameWithoutExtension() + "]");
}
