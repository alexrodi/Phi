/*
  ==============================================================================

    ModuleBox.h
    Created: 8 Feb 2020 12:28:45pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

#include "PortUI.h"
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
    
    /// Sets the highlight colour of the module
    void setHighlightColour(const juce::Colour&);
    
    void setShowPortLabels(ShowPortLabels);
    
    /// Shows the selected state in the UI
    void setSelected(bool selected) { isSelected = selected; repaint(); }
    
    /// Returns -1 if not found
    PortID getPortID(const PortUI&) const;
    
    const PortUI& getPort(PortType, PortID) const;
    
private:
    const float HEADER_HEIGHT = 27.0f;
    const float CONTENT_PADDING = 10.0f;
    const int PLUG_COLUMN_WIDTH = 40;

    /// Our LookAndFeel instance for this module box
    struct ModuleLookAndFeel : juce::LookAndFeel_V4
    {
        ModuleLookAndFeel()
        {
            setHighlightColour(juce::Colours::cyan.withSaturation(0.5f));
            setColour(PhiColourIds::Module::Background, juce::Colours::darkgrey.darker());
            setColour(PhiColourIds::Module::Outline, juce::Colours::grey);
            setColour(PhiColourIds::Module::Text, juce::Colours::grey);
            setColour(PhiColourIds::Module::SelectedOutline, juce::Colours::grey.brighter());
            setColour(PhiColourIds::Module::SelectedText, juce::Colours::grey.brighter());
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
    
    /// The hosted moduleUI
    std::unique_ptr<ModuleUI> moduleUI;
    
    /// The patchable inlets
    std::vector<std::unique_ptr<PortUI>> inlets;
    /// The patchable outlets
    std::vector<std::unique_ptr<PortUI>> outlets;
    
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

    /**
     * Places all Ports equidistant in a column (top -> bottom).
     * @param Ports A vector of Ports to be placed.
     * @param bounds The rectangle to place the Ports within.
     */
    void placePorts(const std::vector<std::unique_ptr<PortUI>>&, juce::Rectangle<int>);

    /**
     * Places the inlets and outlets of this ModuleUI given a rectangle and returns the remaining area.
     * The result is used to bound the module contents via wasResized().
     * @param bounds The module's bounds to place the inlets and outlets within.
     * @return The remaining area after placing the Ports.
     */
    juce::Rectangle<int> placeInletsAndOutlets (juce::Rectangle<int>);
    
    //==================================================================================
    
    /// Callback for Buttons (e.g. power button)
    void buttonClicked (juce::Button*) override;
    
    ///@cond
    void buttonStateChanged (juce::Button*) override {};
    ///@endcond

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ModuleBox)
};
