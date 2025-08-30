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
        Pop,
        Boulder,
        Neon
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
            case Palettes::Pop: setPop(); break;
            case Palettes::Neon: setNeon(); break;
            case Palettes::Boulder: setBoulder(); break;
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
    
    void setNeon() {
        colours.clear();

        // General
        colours[PhiColourIds::General::Background] =         juce::Colour(0xff1a1a2e);
        colours[PhiColourIds::General::TopBar] =             juce::Colour(0xff0f0f1c);

        // Modules
        colours[PhiColourIds::Module::Highlight] =           juce::Colours::deeppink.withSaturation(0.8f);
        colours[PhiColourIds::Module::Lowlight] =            juce::Colour(0xff2a2a44);
        colours[PhiColourIds::Module::DisabledHighlight] =   juce::Colour(0xff555570);
        colours[PhiColourIds::Module::DisabledText] =        juce::Colour(0xff555570);
        colours[PhiColourIds::Module::Background] =          juce::Colour(0xff1f1f33);
        colours[PhiColourIds::Module::Outline] =             juce::Colour(0xff3f3f5a);
        colours[PhiColourIds::Module::SelectedOutline] =     juce::Colour(0xff3f3f5a).brighter();
        colours[PhiColourIds::Module::Name] =                juce::Colour(0xff9999bb);
        colours[PhiColourIds::Module::SelectedName] =        juce::Colour(0xffffffff);
        colours[PhiColourIds::Module::Text] =                juce::Colour(0xffbbbbcc);

        // Ports
        colours[PhiColourIds::Port::IntletOutline] =         juce::Colour(0xff444466);
        colours[PhiColourIds::Port::IntletFill] =            juce::Colour(0xff1f1f33);
        colours[PhiColourIds::Port::OutletOutline] =         juce::Colour(0xff1f1f33);
        colours[PhiColourIds::Port::OutletFill] =            juce::Colour(0xff444466);
        colours[PhiColourIds::Port::Text] =                  juce::Colour(0xffbbbbcc);

        // Connections
        colours[PhiColourIds::Connection::DefaultFill] =     juce::Colour(0xff666688);
        colours[PhiColourIds::Connection::SelectedOutline] = juce::Colour(0xff3f3f5a).brighter().brighter();
    }

    void setPop() {
        colours.clear();
        
        // General
        colours[PhiColourIds::General::Background] =         juce::Colours::darkgrey.darker();
        colours[PhiColourIds::General::TopBar] =             juce::Colours::darkgrey.darker().darker();

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
        colours[PhiColourIds::Connection::DefaultFill] =     juce::Colour::greyLevel(0.4f);
        colours[PhiColourIds::Connection::SelectedOutline] = juce::Colour::greyLevel(0.4f).brighter().brighter();
    }

    void setBoulder() {
        colours.clear();

        // General
        colours[PhiColourIds::General::Background] =         juce::Colour(0xff2d2c2b);
        colours[PhiColourIds::General::TopBar] =             juce::Colour(0xff1f1e1d).withBrightness(0.11f);

        // Modules
        colours[PhiColourIds::Module::Highlight] =           juce::Colours::orange.withSaturation(0.5f);
        colours[PhiColourIds::Module::Lowlight] =            juce::Colour(0xff1f1e1d).withBrightness(0.15f);
        colours[PhiColourIds::Module::DisabledHighlight] =   juce::Colour(0xff5c5a59);
        colours[PhiColourIds::Module::DisabledText] =        juce::Colour(0xff5c5a59);
        colours[PhiColourIds::Module::Background] =          juce::Colour(0xff2c2b2a).withBrightness(0.235f);
        colours[PhiColourIds::Module::Outline] =             juce::Colour(0xff4a4847);
        colours[PhiColourIds::Module::SelectedOutline] =     juce::Colour(0xff4a4847).brighter();
        colours[PhiColourIds::Module::Name] =                juce::Colour(0xff8c8a88);
        colours[PhiColourIds::Module::SelectedName] =        juce::Colour(0xfff0efe9);
        colours[PhiColourIds::Module::Text] =                juce::Colour(0xffb7b4b1);

        // Ports
        colours[PhiColourIds::Port::IntletOutline] =         juce::Colour(0xff5c5a59);
        colours[PhiColourIds::Port::IntletFill] =            juce::Colour(0xff2d2c2b);
        colours[PhiColourIds::Port::OutletOutline] =         juce::Colour(0xff2d2c2b);
        colours[PhiColourIds::Port::OutletFill] =            juce::Colour(0xff5c5a59);
        colours[PhiColourIds::Port::Text] =                  juce::Colour(0xffb7b4b1);

        // Connections
        colours[PhiColourIds::Connection::DefaultFill] =     juce::Colour(0xff7c7a78);
        colours[PhiColourIds::Connection::SelectedOutline] = juce::Colour(0xff4a4847).brighter().brighter();
    }
};
