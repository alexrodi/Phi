/*
  ==============================================================================

    PhiColours.h
    Created: 3 Feb 2021 10:33:51am
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

class PhiColourIds {
    // 256 colours for each class
    enum class ColourClass {
        General = 0x000,     // IDs 0 - 255
        Module = 0x100,      // IDs 256 - 511
        Connection = 0x200,  // IDs 512 - 767
        Port = 0x300,        // IDs 768 - 1023
    };
public:
    struct General {enum Val{
        Background = (int)ColourClass::General,
        Highlight,
        TopBar
    };};

    struct Module {enum Val{
        Background = (int)ColourClass::Module,
        Outline,
        SelectedOutline,
        Name,
        SelectedName,
        Text,
        DisabledText,
        Highlight,
        DisabledHighlight,
        Lowlight
    };};
    
    struct Port {enum Val{
        IntletOutline = (int)ColourClass::Port,
        IntletFill,
        OutletOutline,
        OutletFill,
        Text
    };};

    struct Connection {enum Val{
        DefaultFill = (int)ColourClass::Connection,
        SelectedOutline
    };};
};

inline juce::CallOutBox& openColourSelector(juce::Rectangle<int> area, juce::Colour initialColour, juce::Component* comp, juce::ChangeListener* listener)
{
    auto colourSelector = std::make_unique<juce::ColourSelector>(juce::ColourSelector::showColourspace);
    colourSelector->setSize(150, 130);
    colourSelector->addChangeListener(listener);
    colourSelector->setCurrentColour(initialColour);

    auto& callOutBox = juce::CallOutBox::launchAsynchronously(std::move(colourSelector), area, comp);
    
    callOutBox.setLookAndFeel(&comp->getLookAndFeel());
    callOutBox.setAlwaysOnTop(true);
    
    return callOutBox;
}
