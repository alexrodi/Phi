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
    enum class Palettes {
        Default,
        Slate,
        Dusk,
        Earth,
        Granite
    };

    PhiTheme() { setTheme(Palettes::Default); }
    
    /// Constructs a PhiTheme with a specified palette.
    PhiTheme(Palettes palette) {
        setTheme(palette);
    }

    /// Access a colour by its ID.
    juce::Colour& operator[](int colourId) { return colours[colourId]; }
    const juce::Colour& operator[](int colourId) const { return colours.at(colourId); }

    /// Applies all colours in the theme to a LookAndFeel object.
    void apply(juce::LookAndFeel& lf) const {
        for (auto& [colourId, colour] : colours)
            lf.setColour(colourId, colour);
    }

    /// Sets the current theme from the Palettes enum.
    void setTheme(Palettes palette) {
        switch (palette) {
            case Palettes::Slate:    setSlate(); break;
            case Palettes::Dusk:  setDusk(); break;
            case Palettes::Earth:    setEarth(); break;
            case Palettes::Granite: setGranite(); break;
            case Palettes::Default:
            default:
                setDefault();
                break;
        }
    }

private:
    std::unordered_map<int, juce::Colour> colours;

    void setDefault() {
        colours.clear();
        
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
    
    void setDusk() {
        colours.clear();

        // General
        colours[PhiColourIds::General::Background] =         juce::Colour(0xff1a1a2e); // Deep, dark purple-blue
        colours[PhiColourIds::General::TopBar] =             juce::Colour(0xff0f0f1c); // Even darker purple-blue
        colours[PhiColourIds::General::Highlight] =          juce::Colour(0xff3f3f5a); // Muted purple-grey

        // Modules
        colours[PhiColourIds::Module::Highlight] =           juce::Colours::deeppink.withSaturation(0.8f); // High-saturation highlight
        colours[PhiColourIds::Module::Lowlight] =            juce::Colour(0xff2a2a44); // Dark purple-grey
        colours[PhiColourIds::Module::DisabledHighlight] =   juce::Colour(0xff555570); // Muted grey
        colours[PhiColourIds::Module::DisabledText] =        juce::Colour(0xff555570); // Muted grey
        colours[PhiColourIds::Module::Background] =          juce::Colour(0xff1f1f33); // Dark purple-blue
        colours[PhiColourIds::Module::Outline] =             juce::Colour(0xff3f3f5a); // Muted purple-grey
        colours[PhiColourIds::Module::SelectedOutline] =     juce::Colour(0xff3f3f5a).brighter();
        colours[PhiColourIds::Module::Name] =                juce::Colour(0xff9999bb); // Light grey-blue
        colours[PhiColourIds::Module::SelectedName] =        juce::Colour(0xffffffff); // White for selected text
        colours[PhiColourIds::Module::Text] =                juce::Colour(0xffbbbbcc); // Light grey-blue text

        // Ports
        colours[PhiColourIds::Port::IntletOutline] =         juce::Colour(0xff444466); // Dark blue-grey
        colours[PhiColourIds::Port::IntletFill] =            juce::Colour(0xff1f1f33); // Dark purple-blue
        colours[PhiColourIds::Port::OutletOutline] =         juce::Colour(0xff1f1f33); // Dark purple-blue
        colours[PhiColourIds::Port::OutletFill] =            juce::Colour(0xff444466); // Dark blue-grey
        colours[PhiColourIds::Port::Text] =                  juce::Colour(0xffbbbbcc); // Light grey-blue

        // Connections
        colours[PhiColourIds::Connection::DefaultFill] =     juce::Colour(0xff666688); // Muted connection line
        colours[PhiColourIds::Connection::SelectedOutline] = juce::Colours::gold.withSaturation(0.9f); // Bright gold highlight
    }

    void setSlate() {
        colours.clear();
        
        // General
        colours[PhiColourIds::General::Background] =         juce::Colours::darkgrey.darker();
        colours[PhiColourIds::General::TopBar] =             juce::Colours::darkgrey.darker().darker();
        colours[PhiColourIds::General::Highlight] =          juce::Colour::greyLevel(0.4f);

        // Modules
        colours[PhiColourIds::Module::Highlight] =           juce::Colours::cyan.withSaturation(0.6f);
        colours[PhiColourIds::Module::Lowlight] =            juce::Colour::greyLevel(0.2f);
        colours[PhiColourIds::Module::DisabledHighlight] =   juce::Colours::grey;
        colours[PhiColourIds::Module::DisabledText] =        juce::Colours::grey;
        colours[PhiColourIds::Module::Background] =          juce::Colour::greyLevel(0.15f);
        colours[PhiColourIds::Module::Outline] =             juce::Colour::greyLevel(0.4f);
        colours[PhiColourIds::Module::SelectedOutline] =     juce::Colour::greyLevel(0.4f).brighter();
        colours[PhiColourIds::Module::Name] =                juce::Colour::greyLevel(0.7f);
        colours[PhiColourIds::Module::SelectedName] =        juce::Colours::white;
        colours[PhiColourIds::Module::Text] =                juce::Colour::greyLevel(0.8f);

        // Ports
        colours[PhiColourIds::Port::IntletOutline] =         juce::Colour::greyLevel(0.5f);
        colours[PhiColourIds::Port::IntletFill] =            juce::Colour::greyLevel(0.2f);
        colours[PhiColourIds::Port::OutletOutline] =         juce::Colour::greyLevel(0.2f);
        colours[PhiColourIds::Port::OutletFill] =            juce::Colour::greyLevel(0.5f);
        colours[PhiColourIds::Port::Text] =                  juce::Colour::greyLevel(0.7f);

        // Connections
        colours[PhiColourIds::Connection::DefaultFill] =     juce::Colour::greyLevel(0.6f);
        colours[PhiColourIds::Connection::SelectedOutline] = juce::Colours::cyan.withSaturation(0.6f).brighter();
    }

    /// Populates the style with the "Muted Earth" theme.
    void setEarth() {
        colours.clear();

        // General
        colours[PhiColourIds::General::Background] =         juce::Colour(0xff2d2c2b); // Dark charcoal brown
        colours[PhiColourIds::General::TopBar] =             juce::Colour(0xff1f1e1d); // Very dark brown
        colours[PhiColourIds::General::Highlight] =          juce::Colour(0xff4a4847); // Medium brown-grey

        // Modules
        colours[PhiColourIds::Module::Highlight] =           juce::Colours::orange.withSaturation(0.5f); // Burnt orange highlight
        colours[PhiColourIds::Module::Lowlight] =            juce::Colour(0xff242322); // Very dark brown
        colours[PhiColourIds::Module::DisabledHighlight] =   juce::Colour(0xff5c5a59); // Muted brown
        colours[PhiColourIds::Module::DisabledText] =        juce::Colour(0xff5c5a59); // Muted brown
        colours[PhiColourIds::Module::Background] =          juce::Colour(0xff2c2b2a); // Dark brown-grey
        colours[PhiColourIds::Module::Outline] =             juce::Colour(0xff4a4847); // Medium brown-grey
        colours[PhiColourIds::Module::SelectedOutline] =     juce::Colour(0xff4a4847).brighter();
        colours[PhiColourIds::Module::Name] =                juce::Colour(0xff8c8a88); // Light brown-grey
        colours[PhiColourIds::Module::SelectedName] =        juce::Colour(0xfff0efe9); // Off-white for selected text
        colours[PhiColourIds::Module::Text] =                juce::Colour(0xffb7b4b1); // Light taupe text

        // Ports
        colours[PhiColourIds::Port::IntletOutline] =         juce::Colour(0xff5c5a59); // Muted brown
        colours[PhiColourIds::Port::IntletFill] =            juce::Colour(0xff2d2c2b); // Dark charcoal brown
        colours[PhiColourIds::Port::OutletOutline] =         juce::Colour(0xff2d2c2b); // Dark charcoal brown
        colours[PhiColourIds::Port::OutletFill] =            juce::Colour(0xff5c5a59); // Muted brown
        colours[PhiColourIds::Port::Text] =                  juce::Colour(0xffb7b4b1); // Light taupe

        // Connections
        colours[PhiColourIds::Connection::DefaultFill] =     juce::Colour(0xff7c7a78); // Medium grey-brown
        colours[PhiColourIds::Connection::SelectedOutline] = juce::Colours::orange.withSaturation(0.5f).brighter(); // Brighter burnt orange
    }

    /// Populates the style with the "Smokey Granite" theme.
    void setGranite() {
        colours.clear();

        // General
        colours[PhiColourIds::General::Background] =         juce::Colour(0xff55555c); // Medium-dark blue-grey
        colours[PhiColourIds::General::TopBar] =             juce::Colour(0xff404047); // Darker blue-grey
        colours[PhiColourIds::General::Highlight] =          juce::Colour(0xff85858c); // Medium blue-grey

        // Modules
        colours[PhiColourIds::Module::Highlight] =           juce::Colours::green.withSaturation(0.7f); // Forest green highlight
        colours[PhiColourIds::Module::Lowlight] =            juce::Colour(0xff45454c); // Dark blue-grey
        colours[PhiColourIds::Module::DisabledHighlight] =   juce::Colour(0xff77777e); // Lighter blue-grey
        colours[PhiColourIds::Module::DisabledText] =        juce::Colour(0xff77777e); // Lighter blue-grey
        colours[PhiColourIds::Module::Background] =          juce::Colour(0xff4e4e55); // Dark blue-grey
        colours[PhiColourIds::Module::Outline] =             juce::Colour(0xff77777e); // Lighter blue-grey
        colours[PhiColourIds::Module::SelectedOutline] =     juce::Colour(0xff77777e).brighter();
        colours[PhiColourIds::Module::Name] =                juce::Colour(0xffa0a0a7); // Light blue-grey
        colours[PhiColourIds::Module::SelectedName] =        juce::Colours::white;
        colours[PhiColourIds::Module::Text] =                juce::Colour(0xffc5c5cc); // Light text

        // Ports
        colours[PhiColourIds::Port::IntletOutline] =         juce::Colour(0xff77777e); // Lighter blue-grey
        colours[PhiColourIds::Port::IntletFill] =            juce::Colour(0xff4e4e55); // Dark blue-grey
        colours[PhiColourIds::Port::OutletOutline] =         juce::Colour(0xff4e4e55); // Dark blue-grey
        colours[PhiColourIds::Port::OutletFill] =            juce::Colour(0xff77777e); // Lighter blue-grey
        colours[PhiColourIds::Port::Text] =                  juce::Colour(0xffc5c5cc); // Light text

        // Connections
        colours[PhiColourIds::Connection::DefaultFill] =     juce::Colour(0xff9999a0); // Medium connection line
        colours[PhiColourIds::Connection::SelectedOutline] = juce::Colours::yellow.withSaturation(0.8f); // Bright yellow
    }
//    
//    /// Theme 5: A high-contrast, futuristic theme with neon highlights.
//    void setCyberpunkNeonStyle() {
//        colours.clear();
//        
//        juce::Colour accent = juce::Colours::magenta;
//
//        // General
//        colours[PhiColourIds::General::Background] = juce::Colour::fromFloatRGBA(0.05f, 0.0f, 0.1f, 1.0f); // Deep purple
//        colours[PhiColourIds::General::TopBar] = juce::Colours::black;
//        colours[PhiColourIds::General::Highlight] = juce::Colours::white;
//
//        // Modules
//        colours[PhiColourIds::Module::Highlight] = accent;
//        colours[PhiColourIds::Module::Lowlight] = juce::Colour::fromFloatRGBA(0.1f, 0.05f, 0.15f, 1.0f);
//        colours[PhiColourIds::Module::DisabledHighlight] = juce::Colour::fromFloatRGBA(0.4f, 0.2f, 0.5f, 1.0f);
//        colours[PhiColourIds::Module::DisabledText] = juce::Colour::fromFloatRGBA(0.4f, 0.2f, 0.5f, 1.0f);
//        colours[PhiColourIds::Module::Background] = juce::Colour::fromFloatRGBA(0.1f, 0.0f, 0.15f, 1.0f);
//        colours[PhiColourIds::Module::Outline] = juce::Colour::fromFloatRGBA(0.5f, 0.0f, 0.7f, 1.0f);
//        colours[PhiColourIds::Module::SelectedOutline] = juce::Colours::cyan;
//        colours[PhiColourIds::Module::Name] = accent.darker(0.2f);
//        colours[PhiColourIds::Module::SelectedName] = juce::Colours::cyan;
//        colours[PhiColourIds::Module::Text] = accent.brighter(0.3f);
//
//        // Ports
//        colours[PhiColourIds::Port::IntletOutline] = juce::Colours::cyan;
//        colours[PhiColourIds::Port::IntletFill] = juce::Colours::black;
//        colours[PhiColourIds::Port::OutletOutline] = juce::Colours::black;
//        colours[PhiColourIds::Port::OutletFill] = juce::Colours::cyan;
//        colours[PhiColourIds::Port::Text] = juce::Colours::cyan.darker(0.3f);
//
//        // Connections
//        colours[PhiColourIds::Connection::DefaultFill] = juce::Colours::cyan;
//        colours[PhiColourIds::Connection::SelectedOutline] = accent;
//    }
};
