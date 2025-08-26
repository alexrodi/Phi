
#pragma once

#include "../State.h"
#include "PhiColours.h"
#include "Patcher.h"
#include "component/PhiSliderButton.h"

//==============================================================================
/// The top-most component that includes all the content
class MainComponent   : public juce::Component

{
//==============================================================================
public:
    explicit MainComponent(State&);
    ~MainComponent();

    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    PhiLookAndFeel phiLookAndFeel;
    
    //==============================================================================
    State& state;
    
    /// The main editor, it has the means to create modules, connections and produce the resulting audio
    Patcher patcher;
    
    /// The Viewport component that presents the view of mainPatcher to allow scrolling
    juce::Viewport viewport;
    
    /// A simple button to change the patch-cord drawing method (just because)
    PhiSliderButton patchCordTypeButton;
    
    /// A simple button to toggle between inlet/outlet names being hinted or labeled
    PhiSliderButton showPortLabelsButton;
    
    /// The rectangle that constrains the top bar on the window
    juce::Rectangle<int> topBarBounds;
    
    //==============================================================================
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
