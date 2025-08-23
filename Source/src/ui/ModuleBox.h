/*
  ==============================================================================

    ModuleBox.h
    Created: 8 Feb 2020 12:28:45pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

#include "ModuleUI.h"
#include "PhiColours.h"
#include "component/PhiToggleButton.h"

//==============================================================================
/*
*/
struct ModuleBox : juce::Component,
                   juce::Button::Listener,
                   juce::ComponentBoundsConstrainer
{
    explicit ModuleBox(std::unique_ptr<ModuleUI>);
    ~ModuleBox();

    void paint(juce::Graphics&) override;
    void resized() override;
    
    /// The hosted moduleUI
    std::unique_ptr<ModuleUI> moduleUI;
    
    /// Sets the highlight colour of the module
    void setHighlightColour(const juce::Colour&);
    
    void setShowPortLabels(ShowPortLabels);
    
    void setSelected(bool selected) { isSelected = selected; repaint(); }

private:
    const float HEADER_HEIGHT = 27.0f;
    const float CONTENT_PADDING = 10.0f;
    
    /// Our LookAndFeel instance for this module box
    struct ModuleLookAndFeel  : public juce::LookAndFeel_V4
    {
        ModuleLookAndFeel()
        {
            setHighlightColour(juce::Colours::cyan.withSaturation(0.5f));
            setColour(PhiColourIds::Module::Background, juce::Colours::darkgrey.darker());
            setColour(PhiColourIds::Module::OutlineAndText, juce::Colours::grey);
            setColour(PhiColourIds::Module::SelectedOutlineAndText, juce::Colours::grey.brighter());
            setColour(PhiColourIds::Module::HeaderLine, juce::Colours::grey);
            
            setColour(juce::Slider::thumbColourId, findColour(PhiColourIds::Module::Highlight));
            setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour::greyLevel(0.21f));
            setColour(juce::Slider::rotarySliderFillColourId, juce::Colour::greyLevel(0.17f));
            setColour(juce::Slider::textBoxHighlightColourId, juce::Colour::greyLevel(0.2f));
            setColour(juce::Slider::textBoxTextColourId, juce::Colours::grey.brighter());
            setColour(juce::Slider::textBoxOutlineColourId, juce::Colour()); // no color
            setColour(juce::Label::backgroundWhenEditingColourId, juce::Colour::greyLevel(0.3f));
            setColour(juce::CaretComponent::caretColourId, juce::Colour::greyLevel(0.8f));
            setColour(juce::TextEditor::focusedOutlineColourId, juce::Colour());
            setColour(juce::TextEditor::highlightedTextColourId,juce:: Colour::greyLevel(0.7f));
            setColour(juce::TextButton::textColourOnId, juce::Colours::grey.brighter());
            
            setColour(juce::ColourSelector::backgroundColourId, juce::Colours::transparentBlack);
        }
        
        void setModuleOn(bool isOn)
        {
            if (isOn)
            {
                setColour(juce::Slider::thumbColourId, findColour(PhiColourIds::Module::Highlight));
                setColour(juce::Slider::textBoxTextColourId, juce::Colours::grey.brighter());
                setColour(juce::TextButton::textColourOnId, juce::Colours::grey.brighter());
            }
            else
            {
                setColour(juce::Slider::thumbColourId, juce::Colours::grey);
                setColour(juce::Slider::textBoxTextColourId, juce::Colours::grey);
                setColour(juce::TextButton::textColourOnId, juce::Colours::grey);
            }
        }
        
        /// Sets the highlight colour of the module
        void setHighlightColour(juce::Colour colour)
        {
            setColour(PhiColourIds::Module::Highlight, colour);
        }
        
        
    } lookandfeel;

    /// This box's rectangle
    juce::Rectangle<float> moduleBoxRectangle;
    /// The line dividing the header from the module
    juce::Rectangle<float> headerLine;
    /// The module name's rectangle
    juce::Rectangle<float> nameRectangle;

    //==================================================================================
    
    /// The top-left button for enabling and disabling the contained module
    PhiToggleButton powerButton;

    /// Imposes a draggable corner on the component for resizing
    juce::ResizableCornerComponent resizer;

    //==================================================================================
    
    bool isSelected = false;
    
    //==================================================================================

    /// Callback for Buttons (e.g. power button)
    void buttonClicked (juce::Button*) override;
    
    ///@cond
    void buttonStateChanged (juce::Button*) override {};
    ///@endcond

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ModuleBox)
};
