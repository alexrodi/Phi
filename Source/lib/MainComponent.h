
#pragma once

///@cond
#include <JuceHeader.h>
///@endcond
#include "MainPatcher.h"
#include "UIobjects/ui_SliderButton.h"

//==============================================================================
/// The lowest-order component that includes all the content
class MainComponent   : public Component,
                        public Button::Listener

{
//==============================================================================
public:
    MainComponent(std::unique_ptr<AudioEngine>);
    ~MainComponent();

    void paint (Graphics& g) override;
    void resized() override;

//==============================================================================
private:
    /// The main editor, it has the means to create modules, connections and produce the resulting audio
    MainPatcher mainPatcher;
    
    /// The Viewport component that presents the view of mainPatcher to allow scrolling
    Viewport viewport;
    
    /// A simple button to change the patch-cord drawing method (just because)
    ui_SliderButton patchCordTypeButton;
    
    /// A simple button to toggle between inlet/outlet names being hinted or labeled
    ui_SliderButton inoutNamesTypeButton;
    
    /// The rectangle that constrains the top bar on the window
    Rectangle<int> topBarBounds;
    
    /// The callback for any button on this component
    void buttonClicked (Button*) override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
