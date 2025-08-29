
#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent(State& state, FileManager& fileManager) :
state(state),
patcher(state),
fileMenuModel(fileManager),
fileMenu(&fileMenuModel),
patchCordTypeButton("Gravity"),
showPortLabelsButton("Hint", "Label")
{
    // ======================= Children =======================
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
    
    // ======================= Look and Feel =======================
    juce::LookAndFeel::setDefaultLookAndFeel(&lookandfeel);
    
    lookandfeel.setTheme({}); // <- must be called after setting, otherwise font becomes corrupted
    
    // TODO: Components shouldn't reach for module colours!
    // Components out here have a neutral highlight colour
    lookandfeel.setColour(PhiColourIds::Module::Highlight, lookandfeel.findColour(PhiColourIds::General::Highlight));
    
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
    g.fillRect(topBarBounds);
}

void MainComponent::resized()
{
    auto bounds = getLocalBounds();
    
    topBarBounds = bounds.removeFromTop(38);
    
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

void MainComponent::fileSaved(juce::File file) {
    getTopLevelComponent()->setName("Phi [" + file.getFileNameWithoutExtension() + "]");
}
