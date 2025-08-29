/*
  ==============================================================================

    PhiTheme.h
    Created: 29 Aug 2025 2:42:18am
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

#include "PhiColours.h"

struct PhiTheme {
    ///Construct a new PhiStyle object and initialize it with default colours.
    PhiTheme() {
        setDefaultStyle();
    }

    juce::Colour& operator[](int colourId) { return colours[colourId]; }
    const juce::Colour& operator[](int colourId) const { return colours.at(colourId); }
    
    void apply(juce::LookAndFeel& lf) {
        for (auto& [colourId, colour] : colours)
            lf.setColour(colourId, colour);
    }

private:
    /// Populates the style with a default "dark theme".
    void setDefaultStyle() {
        // General
        colours[PhiColourIds::General::Background] =         juce::Colours::darkgrey;
        colours[PhiColourIds::General::TopBar] =             juce::Colours::darkgrey.darker();
        colours[PhiColourIds::General::Highlight] =          juce::Colour::greyLevel(0.8f);

        // Modules
        colours[PhiColourIds::Module::Highlight] =           juce::Colours::cyan.withSaturation(0.5f);
        colours[PhiColourIds::Module::Lowlight] =            juce::Colour::greyLevel(0.205f);
        colours[PhiColourIds::Module::DisabledHighlight] =   juce::Colours::grey;
        colours[PhiColourIds::Module::DisabledText] =        juce::Colours::grey;
        colours[PhiColourIds::Module::Background] =          juce::Colours::darkgrey.darker();
        colours[PhiColourIds::Module::Outline] =             juce::Colours::grey;
        colours[PhiColourIds::Module::SelectedOutline] =     juce::Colours::grey.brighter();
        colours[PhiColourIds::Module::Name] =                juce::Colours::grey;
        colours[PhiColourIds::Module::SelectedName] =        juce::Colours::grey.brighter();
        colours[PhiColourIds::Module::Text] =                juce::Colours::grey.brighter();

        // Ports
        colours[PhiColourIds::Port::IntletOutline] =         juce::Colours::grey;
        colours[PhiColourIds::Port::IntletFill] =            juce::Colours::darkgrey;
        colours[PhiColourIds::Port::OutletOutline] =         juce::Colours::darkgrey;
        colours[PhiColourIds::Port::OutletFill] =            juce::Colours::grey;
        colours[PhiColourIds::Port::Text] =                  juce::Colours::grey;

        // Connections
        colours[PhiColourIds::Connection::DefaultFill] =     juce::Colours::grey;
        colours[PhiColourIds::Connection::SelectedOutline] = juce::Colours::grey.brighter();
    }
    
    std::unordered_map<int, juce::Colour> colours;
};


