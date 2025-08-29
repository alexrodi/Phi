/*
  ==============================================================================

    PhiTheme.h
    Created: 29 Aug 2025 2:42:18am
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

#include "PhiColours.h"

inline juce::Typeface::Ptr createFont(const juce::String& fontName) {
    return juce::Typeface::createSystemTypefaceFor(juce::FontOptions().withName(fontName));
}


struct PhiTheme {
    enum class Palettes {
        DefaultClassic,
        DefaultDark,
        SolarisLight,
        MidnightBlue,
        VintageAnalog,
        NordicNoir,
        CyberpunkNeon
    };

    /// Constructs a PhiTheme with a specified palette.
    PhiTheme(Palettes palette = Palettes::DefaultClassic) {
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
            case Palettes::SolarisLight:    setSolarisLightStyle(); break;
            case Palettes::MidnightBlue:    setMidnightBlueStyle(); break;
            case Palettes::VintageAnalog: setVintageAnalogStyle(); break;
            case Palettes::NordicNoir:      setNordicNoirStyle(); break;
            case Palettes::CyberpunkNeon: setCyberpunkNeonStyle(); break;
            case Palettes::DefaultDark:     setDefaultDarkStyle(); break;
            case Palettes::DefaultClassic:
            default:
                setDefaultClassicStyle();
                break;
        }
    }

private:
    std::unordered_map<int, juce::Colour> colours;
    juce::Typeface::Ptr font;

    /// Populates the style with the default "classic theme".
    void setDefaultClassicStyle() {
        colours.clear();
        
        font = createFont("Helvetica Neue");
        
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
    
    /// Populates the style with the default "dark theme".
    void setDefaultDarkStyle() {
        colours.clear();
        
        font = createFont("Inter");
        
        // General
        colours[PhiColourIds::General::Background] = juce::Colour::fromFloatRGBA(0.18f, 0.20f, 0.22f, 1.0f);
        colours[PhiColourIds::General::TopBar] = juce::Colour::fromFloatRGBA(0.12f, 0.13f, 0.15f, 1.0f);
        colours[PhiColourIds::General::Highlight] = juce::Colour::fromFloatRGBA(0.9f, 0.9f, 0.9f, 1.0f);

        // Modules
        colours[PhiColourIds::Module::Highlight] = juce::Colours::cyan.withSaturation(0.5f);
        colours[PhiColourIds::Module::Lowlight] = juce::Colour::greyLevel(0.205f);
        colours[PhiColourIds::Module::DisabledHighlight] = juce::Colours::grey;
        colours[PhiColourIds::Module::DisabledText] = juce::Colours::grey;
        colours[PhiColourIds::Module::Background] = juce::Colour::fromFloatRGBA(0.12f, 0.13f, 0.15f, 1.0f);
        colours[PhiColourIds::Module::Outline] = juce::Colours::grey.darker(0.5f);
        colours[PhiColourIds::Module::SelectedOutline] = juce::Colours::grey.brighter(0.8f);
        colours[PhiColourIds::Module::Name] = juce::Colours::grey;
        colours[PhiColourIds::Module::SelectedName] = juce::Colours::grey.brighter();
        colours[PhiColourIds::Module::Text] = juce::Colours::grey.brighter();

        // Ports
        colours[PhiColourIds::Port::IntletOutline] = juce::Colours::grey;
        colours[PhiColourIds::Port::IntletFill] = juce::Colours::darkgrey;
        colours[PhiColourIds::Port::OutletOutline] = juce::Colours::darkgrey;
        colours[PhiColourIds::Port::OutletFill] = juce::Colours::grey;
        colours[PhiColourIds::Port::Text] = juce::Colours::grey;

        // Connections
        colours[PhiColourIds::Connection::DefaultFill] = juce::Colours::grey;
        colours[PhiColourIds::Connection::SelectedOutline] = juce::Colours::grey.brighter();
    }

    /// Theme 1: A bright, clean, and modern light theme.
    void setSolarisLightStyle() {
        colours.clear();
        
        font = createFont("Lato");
        
        juce::Colour accent = juce::Colour::fromFloatRGBA(0.0f, 0.46f, 0.8f, 1.0f); // A nice, modern blue

        // General
        colours[PhiColourIds::General::Background] = juce::Colour::fromFloatRGBA(0.94f, 0.95f, 0.96f, 1.0f);
        colours[PhiColourIds::General::TopBar] = juce::Colour::fromFloatRGBA(0.88f, 0.89f, 0.90f, 1.0f);
        colours[PhiColourIds::General::Highlight] = juce::Colour::fromFloatRGBA(0.1f, 0.1f, 0.1f, 1.0f);

        // Modules
        colours[PhiColourIds::Module::Highlight] = accent;
        colours[PhiColourIds::Module::Lowlight] = juce::Colour::greyLevel(0.9f);
        colours[PhiColourIds::Module::DisabledHighlight] = juce::Colour::fromFloatRGBA(0.75f, 0.75f, 0.75f, 1.0f);
        colours[PhiColourIds::Module::DisabledText] = juce::Colour::fromFloatRGBA(0.6f, 0.6f, 0.6f, 1.0f);
        colours[PhiColourIds::Module::Background] = juce::Colours::white;
        colours[PhiColourIds::Module::Outline] = juce::Colour::fromFloatRGBA(0.8f, 0.8f, 0.8f, 1.0f);
        colours[PhiColourIds::Module::SelectedOutline] = accent.brighter(0.2f);
        colours[PhiColourIds::Module::Name] = juce::Colour::fromFloatRGBA(0.4f, 0.4f, 0.4f, 1.0f);
        colours[PhiColourIds::Module::SelectedName] = juce::Colour::fromFloatRGBA(0.1f, 0.1f, 0.1f, 1.0f);
        colours[PhiColourIds::Module::Text] = juce::Colour::fromFloatRGBA(0.2f, 0.2f, 0.2f, 1.0f);

        // Ports
        colours[PhiColourIds::Port::IntletOutline] = juce::Colour::fromFloatRGBA(0.6f, 0.6f, 0.6f, 1.0f);
        colours[PhiColourIds::Port::IntletFill] = juce::Colour::fromFloatRGBA(0.9f, 0.9f, 0.9f, 1.0f);
        colours[PhiColourIds::Port::OutletOutline] = juce::Colour::fromFloatRGBA(0.9f, 0.9f, 0.9f, 1.0f);
        colours[PhiColourIds::Port::OutletFill] = juce::Colour::fromFloatRGBA(0.6f, 0.6f, 0.6f, 1.0f);
        colours[PhiColourIds::Port::Text] = juce::Colour::fromFloatRGBA(0.4f, 0.4f, 0.4f, 1.0f);

        // Connections
        colours[PhiColourIds::Connection::DefaultFill] = juce::Colour::fromFloatRGBA(0.5f, 0.5f, 0.5f, 1.0f);
        colours[PhiColourIds::Connection::SelectedOutline] = accent;
    }

    /// Theme 2: A deep, cinematic dark theme with blue accents.
    void setMidnightBlueStyle() {
        colours.clear();
        
        font = createFont("Montserrat");
        
        juce::Colour accent = juce::Colour::fromFloatRGBA(0.3f, 0.6f, 1.0f, 1.0f); // Electric Blue

        // General
        colours[PhiColourIds::General::Background] = juce::Colour::fromFloatRGBA(0.08f, 0.09f, 0.12f, 1.0f);
        colours[PhiColourIds::General::TopBar] = juce::Colours::black;
        colours[PhiColourIds::General::Highlight] = juce::Colour::fromFloatRGBA(0.95f, 0.95f, 1.0f, 1.0f);

        // Modules
        colours[PhiColourIds::Module::Highlight] = accent;
        colours[PhiColourIds::Module::Lowlight] = juce::Colour::fromFloatRGBA(0.1f, 0.11f, 0.14f, 1.0f);
        colours[PhiColourIds::Module::DisabledHighlight] = juce::Colour::fromFloatRGBA(0.4f, 0.4f, 0.45f, 1.0f);
        colours[PhiColourIds::Module::DisabledText] = juce::Colour::fromFloatRGBA(0.4f, 0.4f, 0.45f, 1.0f);
        colours[PhiColourIds::Module::Background] = juce::Colour::fromFloatRGBA(0.12f, 0.14f, 0.18f, 1.0f);
        colours[PhiColourIds::Module::Outline] = juce::Colour::fromFloatRGBA(0.2f, 0.22f, 0.28f, 1.0f);
        colours[PhiColourIds::Module::SelectedOutline] = accent.brighter(0.4f);
        colours[PhiColourIds::Module::Name] = juce::Colour::fromFloatRGBA(0.6f, 0.6f, 0.7f, 1.0f);
        colours[PhiColourIds::Module::SelectedName] = juce::Colours::white;
        colours[PhiColourIds::Module::Text] = juce::Colour::fromFloatRGBA(0.8f, 0.8f, 0.9f, 1.0f);

        // Ports
        colours[PhiColourIds::Port::IntletOutline] = juce::Colour::fromFloatRGBA(0.6f, 0.6f, 0.7f, 1.0f);
        colours[PhiColourIds::Port::IntletFill] = juce::Colour::fromFloatRGBA(0.08f, 0.09f, 0.12f, 1.0f);
        colours[PhiColourIds::Port::OutletOutline] = juce::Colour::fromFloatRGBA(0.08f, 0.09f, 0.12f, 1.0f);
        colours[PhiColourIds::Port::OutletFill] = juce::Colour::fromFloatRGBA(0.6f, 0.6f, 0.7f, 1.0f);
        colours[PhiColourIds::Port::Text] = juce::Colour::fromFloatRGBA(0.6f, 0.6f, 0.7f, 1.0f);

        // Connections
        colours[PhiColourIds::Connection::DefaultFill] = juce::Colour::fromFloatRGBA(0.7f, 0.7f, 0.8f, 1.0f);
        colours[PhiColourIds::Connection::SelectedOutline] = accent.brighter();
    }

    /// Theme 3: Inspired by vintage analog hardware with warm, creamy tones.
    void setVintageAnalogStyle() {
        colours.clear();
        
        font = createFont("Roboto Slab");
        
        juce::Colour accent = juce::Colours::orange.darker(0.1f);

        // General
        colours[PhiColourIds::General::Background] = juce::Colour::fromFloatRGBA(0.85f, 0.82f, 0.78f, 1.0f); // Cream
        colours[PhiColourIds::General::TopBar] = juce::Colour::fromFloatRGBA(0.25f, 0.2f, 0.18f, 1.0f); // Dark Wood
        colours[PhiColourIds::General::Highlight] = juce::Colour::fromFloatRGBA(0.1f, 0.1f, 0.1f, 1.0f);

        // Modules
        colours[PhiColourIds::Module::Highlight] = accent;
        colours[PhiColourIds::Module::Lowlight] = juce::Colour::fromFloatRGBA(0.7f, 0.68f, 0.65f, 1.0f);
        colours[PhiColourIds::Module::DisabledHighlight] = juce::Colour::fromFloatRGBA(0.6f, 0.58f, 0.55f, 1.0f);
        colours[PhiColourIds::Module::DisabledText] = juce::Colour::fromFloatRGBA(0.5f, 0.48f, 0.45f, 1.0f);
        colours[PhiColourIds::Module::Background] = juce::Colour::fromFloatRGBA(0.4f, 0.4f, 0.4f, 1.0f); // Grey metal
        colours[PhiColourIds::Module::Outline] = juce::Colour::fromFloatRGBA(0.3f, 0.3f, 0.3f, 1.0f);
        colours[PhiColourIds::Module::SelectedOutline] = juce::Colours::black;
        colours[PhiColourIds::Module::Name] = juce::Colour::fromFloatRGBA(0.9f, 0.9f, 0.9f, 1.0f);
        colours[PhiColourIds::Module::SelectedName] = juce::Colours::white;
        colours[PhiColourIds::Module::Text] = juce::Colours::white;

        // Ports
        colours[PhiColourIds::Port::IntletOutline] = juce::Colours::black;
        colours[PhiColourIds::Port::IntletFill] = juce::Colour::fromFloatRGBA(0.3f, 0.3f, 0.3f, 1.0f);
        colours[PhiColourIds::Port::OutletOutline] = juce::Colour::fromFloatRGBA(0.3f, 0.3f, 0.3f, 1.0f);
        colours[PhiColourIds::Port::OutletFill] = juce::Colours::black;
        colours[PhiColourIds::Port::Text] = juce::Colours::white;

        // Connections
        colours[PhiColourIds::Connection::DefaultFill] = juce::Colour::fromFloatRGBA(0.2f, 0.2f, 0.2f, 1.0f);
        colours[PhiColourIds::Connection::SelectedOutline] = accent;
    }

    /// Theme 4: A cool, minimalist, and professional palette.
    void setNordicNoirStyle() {
        colours.clear();
        
        font = createFont("Lato");
        
        juce::Colour accent = juce::Colour::fromFloatRGBA(0.36f, 0.72f, 0.72f, 1.0f); // Muted Teal

        // General
        colours[PhiColourIds::General::Background] = juce::Colour::fromFloatRGBA(0.23f, 0.25f, 0.28f, 1.0f);
        colours[PhiColourIds::General::TopBar] = juce::Colour::fromFloatRGBA(0.18f, 0.20f, 0.22f, 1.0f);
        colours[PhiColourIds::General::Highlight] = juce::Colour::fromFloatRGBA(0.9f, 0.92f, 0.95f, 1.0f);

        // Modules
        colours[PhiColourIds::Module::Highlight] = accent;
        colours[PhiColourIds::Module::Lowlight] = juce::Colour::fromFloatRGBA(0.26f, 0.28f, 0.31f, 1.0f);
        colours[PhiColourIds::Module::DisabledHighlight] = juce::Colour::fromFloatRGBA(0.4f, 0.42f, 0.45f, 1.0f);
        colours[PhiColourIds::Module::DisabledText] = juce::Colour::fromFloatRGBA(0.5f, 0.52f, 0.55f, 1.0f);
        colours[PhiColourIds::Module::Background] = juce::Colour::fromFloatRGBA(0.20f, 0.22f, 0.25f, 1.0f);
        colours[PhiColourIds::Module::Outline] = juce::Colour::fromFloatRGBA(0.3f, 0.32f, 0.35f, 1.0f);
        colours[PhiColourIds::Module::SelectedOutline] = accent.brighter(0.5f);
        colours[PhiColourIds::Module::Name] = juce::Colour::fromFloatRGBA(0.6f, 0.62f, 0.65f, 1.0f);
        colours[PhiColourIds::Module::SelectedName] = juce::Colour::fromFloatRGBA(0.9f, 0.92f, 0.95f, 1.0f);
        colours[PhiColourIds::Module::Text] = juce::Colour::fromFloatRGBA(0.8f, 0.82f, 0.85f, 1.0f);

        // Ports
        colours[PhiColourIds::Port::IntletOutline] = juce::Colour::fromFloatRGBA(0.6f, 0.62f, 0.65f, 1.0f);
        colours[PhiColourIds::Port::IntletFill] = juce::Colour::fromFloatRGBA(0.23f, 0.25f, 0.28f, 1.0f);
        colours[PhiColourIds::Port::OutletOutline] = juce::Colour::fromFloatRGBA(0.23f, 0.25f, 0.28f, 1.0f);
        colours[PhiColourIds::Port::OutletFill] = juce::Colour::fromFloatRGBA(0.6f, 0.62f, 0.65f, 1.0f);
        colours[PhiColourIds::Port::Text] = juce::Colour::fromFloatRGBA(0.6f, 0.62f, 0.65f, 1.0f);

        // Connections
        colours[PhiColourIds::Connection::DefaultFill] = juce::Colour::fromFloatRGBA(0.7f, 0.72f, 0.75f, 1.0f);
        colours[PhiColourIds::Connection::SelectedOutline] = accent.brighter(0.2f);
    }
    
    /// Theme 5: A high-contrast, futuristic theme with neon highlights.
    void setCyberpunkNeonStyle() {
        colours.clear();
        
        font = createFont("Major Mono Display");
        
        juce::Colour accent = juce::Colours::magenta;

        // General
        colours[PhiColourIds::General::Background] = juce::Colour::fromFloatRGBA(0.05f, 0.0f, 0.1f, 1.0f); // Deep purple
        colours[PhiColourIds::General::TopBar] = juce::Colours::black;
        colours[PhiColourIds::General::Highlight] = juce::Colours::white;

        // Modules
        colours[PhiColourIds::Module::Highlight] = accent;
        colours[PhiColourIds::Module::Lowlight] = juce::Colour::fromFloatRGBA(0.1f, 0.05f, 0.15f, 1.0f);
        colours[PhiColourIds::Module::DisabledHighlight] = juce::Colour::fromFloatRGBA(0.4f, 0.2f, 0.5f, 1.0f);
        colours[PhiColourIds::Module::DisabledText] = juce::Colour::fromFloatRGBA(0.4f, 0.2f, 0.5f, 1.0f);
        colours[PhiColourIds::Module::Background] = juce::Colour::fromFloatRGBA(0.1f, 0.0f, 0.15f, 1.0f);
        colours[PhiColourIds::Module::Outline] = juce::Colour::fromFloatRGBA(0.5f, 0.0f, 0.7f, 1.0f);
        colours[PhiColourIds::Module::SelectedOutline] = juce::Colours::cyan;
        colours[PhiColourIds::Module::Name] = accent.darker(0.2f);
        colours[PhiColourIds::Module::SelectedName] = juce::Colours::cyan;
        colours[PhiColourIds::Module::Text] = accent.brighter(0.3f);

        // Ports
        colours[PhiColourIds::Port::IntletOutline] = juce::Colours::cyan;
        colours[PhiColourIds::Port::IntletFill] = juce::Colours::black;
        colours[PhiColourIds::Port::OutletOutline] = juce::Colours::black;
        colours[PhiColourIds::Port::OutletFill] = juce::Colours::cyan;
        colours[PhiColourIds::Port::Text] = juce::Colours::cyan.darker(0.3f);

        // Connections
        colours[PhiColourIds::Connection::DefaultFill] = juce::Colours::cyan;
        colours[PhiColourIds::Connection::SelectedOutline] = accent;
    }
};
