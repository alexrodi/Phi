
#pragma once

#include "../State.h"
#include "PhiColours.h"
#include "Patcher.h"
#include "component/PhiSliderButton.h"

//==============================================================================
/// The top-most component that includes all the content
class MainComponent   : public juce::Component,
                        public juce::Button::Listener

{
//==============================================================================
public:
    explicit MainComponent(State&);
    ~MainComponent();

    void paint (juce::Graphics& g) override;
    void resized() override;
    
    struct PhiLookAndFeel  : public juce::LookAndFeel_V4
    {
        PhiLookAndFeel()
        {
            // PopupMenu
            setColour(juce::PopupMenu::textColourId, juce::Colours::lightgrey);
            setColour(juce::PopupMenu::backgroundColourId, juce::Colours::darkgrey.darker());
            setColour(juce::PopupMenu::highlightedBackgroundColourId, juce::Colour::greyLevel(0.2));
            
            // Texbutton
            setColour(juce::TextButton::textColourOnId , juce::Colours::grey.brighter());
            setColour(juce::TextButton::buttonColourId , juce::Colours::transparentBlack);
            
            // Scrollbar
            setColour(juce::ScrollBar::thumbColourId, juce::Colours::lightgrey.withAlpha(0.5f));
            
            // Knob
            setColour(juce::Slider::thumbColourId, juce::Colour::greyLevel(0.8f));
            
            // Hints
            setColour(juce::TooltipWindow::backgroundColourId, juce::Colours::grey);
            setColour(juce::TooltipWindow::textColourId, juce::Colours::darkgrey.darker());
            
            // Colour Selector
            setColour(juce::ColourSelector::backgroundColourId, juce::Colours::transparentBlack);
            
            // Modules
            setColour(PhiColourIds::Module::Background, juce::Colours::darkgrey.darker());
            setColour(PhiColourIds::Module::Outline, juce::Colours::grey);
            setColour(PhiColourIds::Module::Text, juce::Colours::grey);
            setColour(PhiColourIds::Module::SelectedOutline, juce::Colours::grey.brighter());
            setColour(PhiColourIds::Module::SelectedText, juce::Colours::grey.brighter());
            setColour(PhiColourIds::Module::HeaderLine, juce::Colours::grey);
            
            // Connections
            setColour(PhiColourIds::Connection::DefaultFill, juce::Colours::grey);
            setColour(PhiColourIds::Connection::SelectedOutline, juce::Colours::grey.brighter());
            
            LookAndFeel::getDefaultLookAndFeel().setDefaultSansSerifTypefaceName ("Helvetica Neue");
        }
        
        void drawCallOutBoxBackground (juce::CallOutBox& box, juce::Graphics& g, const juce::Path&, juce::Image&) override {
            auto bounds = box.getLocalBounds().reduced(15).toFloat();
            
            g.setColour (findColour(PhiColourIds::Module::Background));
            g.fillRoundedRectangle(bounds, 2.0f);
            
            g.setColour (findColour(PhiColourIds::Module::SelectedOutline));
            g.drawRoundedRectangle(bounds, 2.0f, 2.0f);
        };
        
        int getPopupMenuBorderSize() override { return 1; }
        
    } phiLookAndFeel;


private:
    //==============================================================================
    State& state;
    
    /// The main editor, it has the means to create modules, connections and produce the resulting audio
    Patcher mainPatcher;
    
    /// The Viewport component that presents the view of mainPatcher to allow scrolling
    juce::Viewport viewport;
    
    /// A simple button to change the patch-cord drawing method (just because)
    PhiSliderButton patchCordTypeButton;
    
    /// A simple button to toggle between inlet/outlet names being hinted or labeled
    PhiSliderButton showPortLabelsButton;
    
    /// The rectangle that constrains the top bar on the window
    juce::Rectangle<int> topBarBounds;
    
    //==============================================================================
    
    /// The callback for any button on this component
    void buttonClicked (juce::Button*) override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
