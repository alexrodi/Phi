/*
  ==============================================================================

    PhiColours.h
    Created: 3 Feb 2021 10:33:51am
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

namespace PhiColourIds {
    namespace General {
        enum Val { Background = 0x000, TopBar };
    }

    namespace Module {
        enum Val {
            Background = 0x100,
            Outline,
            SelectedOutline,
            Name,
            SelectedName,
            Text,
            DisabledText,
            Highlight,
            DisabledHighlight,
            Lowlight
        };
    }

    namespace Port {
        enum Val {
            IntletOutline = 0x200,
            IntletFill,
            OutletOutline,
            OutletFill,
            Text
        };
    }

    namespace Connection {
        enum Val { DefaultFill = 0x300, SelectedOutline };
    }
} // namespace PhiColourIDs

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
