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
#include "PhiLookAndFeel.h"
#include "component/PhiToggleButton.h"

//==============================================================================
/*
*/
struct ModuleBox : juce::Component,
                   State::Listener
{
    ModuleBox(State& state, ModuleID moduleID, std::unique_ptr<ModuleUI>);
    ~ModuleBox();

    void paint(juce::Graphics&) override;
    void resized() override;
    void moved() override;
    
    void setSelected(bool selected) { isSelected = selected; repaint(); }
    
    void setShowPortLabels(ShowPortLabels);
    
    /// Returns -1 if not found
    PortID getPortID(const PortUI&) const;
    
    const PortUI& getPort(PortType, PortID) const;
    
private:
    const float padding = 10.0f;
    const float headerHeight = 29.0f;
    const float outlineThickness = 0.5f;
    const float selectedOutlineThickness = 2.0f;
    const float roundness = 2.0f;
    const int powerButtonSize = 15;

    /// Our LookAndFeel class and instance for this module box
    struct ModuleLookAndFeel : PhiLookAndFeel
    {
        ModuleLookAndFeel() :
        highlight(findColour(PhiColourIds::Module::Highlight)),
        text(findColour(PhiColourIds::Module::Text))
        {}
        
        void setModuleOn(bool isOn) {
            setColour(PhiColourIds::Module::Highlight, isOn ? highlight : findColour(PhiColourIds::Module::DisabledHighlight));
            setColour(PhiColourIds::Module::Text, isOn ? text : findColour(PhiColourIds::Module::DisabledText));
            wasOn = isOn;
        }
        
        /// Sets the highlight colour of the module
        void setCustomHighlightColour(juce::Colour colour) {
            highlight = colour;
            setModuleOn(wasOn);
            hasCustomHighlight = true;
        }
        
        void setTheme(const PhiTheme& theme) {
            PhiLookAndFeel::setTheme(theme, false);
            
            if (hasCustomHighlight)
                setCustomHighlightColour(highlight);
        }
        
    private:
        juce::Colour highlight, text;
        bool wasOn = true, hasCustomHighlight = false;
    } lookandfeel;
    
    State& state;
    
    /// The hosted moduleUI
    std::unique_ptr<ModuleUI> moduleUI;
    
    /// The patchable inlets
    std::vector<std::unique_ptr<PortUI>> inlets;
    /// The patchable outlets
    std::vector<std::unique_ptr<PortUI>> outlets;
    
    /// This box's rectangle
    juce::Rectangle<float> boxBounds;
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
    int portColumnWidth = 50;
    int numInletsConnected = 0;
    int numOutletsConnected = 0;
    bool isSelected = false, isCollapsed = false;
    bool resizeIsReentrant = false;
    
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
    
    void handleCollapse();
    void drawBox(juce::Graphics&);
    juce::Path getCollapsedBox();
    void enforceSizeLimits();
    void setTheme();
    
    void colourChanged() override;
    void parentHierarchyChanged() override;
    
    //==================================================================================

    void moduleEnabledChanged(ModuleID, bool) override;
    void moduleColourChanged(ModuleID, const juce::Colour&) override;
    void connectionCreated(ConnectionID) override;
    void connectionDeleted(ConnectionID) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ModuleBox)
};
