/*
  ==============================================================================

    ModuleUI.h
    Created: 21 Apr 2020 7:39:35pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

///@cond
#include <JuceHeader.h>
///@endcond
#include "Plug.h"
class ModuleProcessor;

//==============================================================================
/**
 The base class for all modules' UI view
*/
class ModuleUI    : public AudioProcessorEditor
{
public:

    /** The properties that define a module. */
    struct Props
    {
        /** This module's name. */
        const String name;
        /** A vector of inlet names to display in the UI (will correspond to the processor's inlets). */
        const std::vector<String> inlets;
        /** A vector of outlet names to display in the UI (will correspond to the processor's outlets). */
        const std::vector<String> outlets;
        /** The default width to use when creating this module. */
        const int width;
        /** The default height to use when creating this module. */
        const int height;
        /** The minimum height that the module should have to present its UI, any less, and the ModuleBox will collapse to only display the header. */
        const int minimumHeight;
        /** A reference to the ModuleProcessor that spawned this UI. */
        ModuleProcessor& processor;
    };

    ModuleUI(Props);
    ~ModuleUI();

    /**
     * This module's properties.
     * @see ModuleUI::Props
     */
    Props props;

    std::vector<Plug> inlets;
    std::vector<Plug> outlets;

    /** The NodeID belonging to the parent module, this is used for referencing the module's processing node in the audio graph. */
    AudioProcessorGraph::NodeID nodeID;

    /**
     * The function used by derived classes of ModuleUI to resize their contents.
     * @param bounds The bounds in which to place the content of the derived class
     */
    virtual void onResize (Rectangle<int>) = 0;

private:
    const int PLUG_COLUMN_WIDTH = 40;

    /**
     * Places all plugs equidistant in a column (top -> bottom).
     * @param plugs A vector of plugs to be placed.
     * @param bounds The rectangle to place the plugs within.
     */
    void placePlugs(std::vector<Plug>&, Rectangle<int>);

    /**
     * Places the inlets and outlets of this ModuleUI given a rectangle and returns the remaining area.
     * The result is used to bound the module contents via wasResized().
     * @param bounds The module's bounds to place the inlets and outlets within.
     * @return The remaining area after placing the plugs.
     */
    Rectangle<int> placeInletsAndOutlets (Rectangle<int>);
    
    /** This base-class overrides resized() and exposes wasResized(), containing the actual module's rectangle. */
    void resized() override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ModuleUI)
};
