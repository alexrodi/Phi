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
                   State::Listener,
                   juce::ComponentBoundsConstrainer
{
    ModuleBox(State& state, ModuleID moduleID, std::unique_ptr<ModuleUI>);
    ~ModuleBox();

    void paint(juce::Graphics&) override;
    void resized() override;
    void moved() override;
    
    /// Sets the highlight colour of the module
    void setHighlightColour(const juce::Colour&);
    
    /// Shows the selected state in the UI
    void setSelected(bool selected) { isSelected = selected; repaint(); }
    
    /// Returns -1 if not found
    PortID getPortID(const PortUI&) const;
    
    const PortUI& getPort(PortType, PortID) const;
    
private:
    const float headerHeight = 27.0f;
    const float padding = 10.0f;
    const int portColumnWidth = 50;

    /// Our LookAndFeel class and instance for this module box
    struct ModuleLookAndFeel : PhiLookAndFeel
    {
        ModuleLookAndFeel() {
            setModuleOn(true);
        }
        
        void setModuleOn(bool isOn) {
            if (isOn) {
                setColour(juce::Slider::thumbColourId,       findColour(PhiColourIds::Module::Highlight));
                setColour(juce::Slider::textBoxTextColourId, juce::Colours::grey.brighter());
                setColour(juce::TextButton::textColourOnId,  juce::Colours::grey.brighter());
            } else {
                setColour(juce::Slider::thumbColourId,       juce::Colours::grey);
                setColour(juce::Slider::textBoxTextColourId, juce::Colours::grey);
                setColour(juce::TextButton::textColourOnId,  juce::Colours::grey);
            }
        }
        
        /// Sets the highlight colour of the module
        void setHighlightColour(juce::Colour colour) {
            setColour(PhiColourIds::Module::Highlight, colour);
        }
    } lookandfeel;
    
    State& state;
    
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
    
    ModuleID moduleID;
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
    
    bool handleCollapse();
    
    //==================================================================================
    
    void moduleEnabledChanged(ModuleID, bool) override;
    void showPortLabelsChanged(ShowPortLabels show) override;
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ModuleBox)
};
