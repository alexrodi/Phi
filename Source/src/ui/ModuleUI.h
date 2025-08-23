/*
  ==============================================================================

    ModuleUI.h
    Created: 21 Apr 2020 7:39:35pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

#include "PortUI.h"

class ModuleProcessor;

//==============================================================================
/**
 The base class for all modules' UI view
*/
class ModuleUI    : public juce::AudioProcessorEditor
{
public:

    /** The properties that define a module. */
    struct Props
    {
        /** This module's name. */
        const juce::String name;
        /** A vector of inlet names to display in the UI (will correspond to the processor's inlets). */
        const juce::StringArray inlets;
        /** A vector of outlet names to display in the UI (will correspond to the processor's outlets). */
        const juce::StringArray outlets;
        /** The default width to use when creating this module. */
        const int width;
        /** The default height to use when creating this module. */
        const int height;
        /** The minimum height that the module should have to present its UI, any less, and the ModuleBox will collapse to only display the header. */
        const int minimumHeight;
        /** A reference to the ModuleProcessor that spawned this UI. */
        ModuleProcessor& processor;
    };
    
    ModuleUI(Props arguments);
    ~ModuleUI();

    void resized() override;
    
    /**
     * This module's properties.
     * @see ModuleUI::Props
     */
    Props props;

    std::vector<PortUI> inlets;
    std::vector<PortUI> outlets;
    
    void setShowPortLabels(ShowPortLabels);
    
    int getPortIndex(const PortUI& port) const {
        auto& v = port.getType() == PortType::Inlet ? inlets : outlets;
        
        auto it = std::find_if(v.begin(), v.end(), [&] (auto& item) { return &item == &port; });

        if (it == v.end()) return -1;
        
        return (int)std::distance(v.begin(), it);
    }

protected:
    /**
     * The function used by derived classes of ModuleUI to resize their contents.
     * @param bounds The bounds in which to place the content of the derived class
     */
    virtual void onResize (juce::Rectangle<int>) = 0;

private:
    const int PLUG_COLUMN_WIDTH = 40;

    /**
     * Places all Ports equidistant in a column (top -> bottom).
     * @param Ports A vector of Ports to be placed.
     * @param bounds The rectangle to place the Ports within.
     */
    void placePorts(std::vector<PortUI>&, juce::Rectangle<int>);

    /**
     * Places the inlets and outlets of this ModuleUI given a rectangle and returns the remaining area.
     * The result is used to bound the module contents via wasResized().
     * @param bounds The module's bounds to place the inlets and outlets within.
     * @return The remaining area after placing the Ports.
     */
    juce::Rectangle<int> placeInletsAndOutlets (juce::Rectangle<int>);
    
    /** This base-class overrides resized() and exposes wasResized(), containing
     * the bounds for the derived class to use when placing its components.
     */
    void wasResized() {}
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ModuleUI)
};
