/*
  ==============================================================================

    ModuleUI.h
    Created: 21 Apr 2020 7:39:35pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

#include "../dsp/ModuleProcessor.h"

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
        /** The default size to use when creating this module. */
        const struct {int width, height;} defaultSize;
        /** The minimum size that the module should have to present its UI.
            Blow this width, the ModuleBox will collapse and only display the Ports.
            Below this height, the ModuleBox will collapse and only display the header.
         */
        const struct {int width, height;} minimumSize;
        /** A reference to the ModuleProcessor that spawned this UI. */
        ModuleProcessor& processor;
    };
    
    ModuleUI(Props arguments) :
    AudioProcessorEditor(arguments.processor),
    props(arguments)
    {
        setPaintingIsUnclipped(true);
        setBufferedToImage(true);
    }
    
    ~ModuleUI() {
        props.processor.editorBeingDeleted(this);
    }
    
    /**
     * This module's properties.
     * @see ModuleUI::Props
     */
    Props props;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ModuleUI)
};
