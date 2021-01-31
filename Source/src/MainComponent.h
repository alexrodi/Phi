
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
    
    class PhiLookAndFeel  : public LookAndFeel_V4
    {
        enum ColourIds {
            PlugIntletOutline,
            PlugIntletCenter,
            PlugOutletCenter,
            PlugOutletOutline,
            PlugText
        };
    public:
        
        PhiLookAndFeel()
        {
            // PopupMenu
            setColour(PopupMenu::textColourId, Colours::lightgrey);
            setColour(PopupMenu::backgroundColourId, Colours::darkgrey.darker());
            setColour(PopupMenu::highlightedBackgroundColourId, Colour::greyLevel(0.2));
            
            // Texbutton
            setColour(TextButton::textColourOnId , Colours::grey.brighter());
            setColour(TextButton::buttonColourId , Colours::transparentBlack);
            
            // Scrollbar
            setColour(ScrollBar::thumbColourId, Colours::lightgrey.withAlpha(0.5f));
            
            // Knob
            setColour(Slider::thumbColourId, Colour::greyLevel(0.8f));
            
            // Hints
            setColour(TooltipWindow::outlineColourId, Colours::transparentBlack);
            setColour(TooltipWindow::backgroundColourId, Colours::grey);
            setColour(TooltipWindow::textColourId, Colours::darkgrey.darker());
            
            LookAndFeel::getDefaultLookAndFeel().setDefaultSansSerifTypefaceName ("Helvetica Neue");
        }
    } phiLookAndFeel;


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
