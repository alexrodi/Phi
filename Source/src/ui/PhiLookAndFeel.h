/*
  ==============================================================================

    PhiLookAndFeel.h
    Created: 29 Aug 2025 2:44:15am
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

#include "PhiColours.h"
#include "PhiTheme.h"

struct PhiLookAndFeel  : public juce::LookAndFeel_V4
{
    PhiLookAndFeel()
    {
        setTheme({});
        
        setColour(juce::PopupMenu::highlightedTextColourId,       findColour(PhiColourIds::Module::Text).withMultipliedBrightness(1.3f));
        setColour(juce::PopupMenu::textColourId,                  findColour(PhiColourIds::Module::Text));
        setColour(juce::PopupMenu::backgroundColourId,            findColour(PhiColourIds::Module::Background));
        setColour(juce::PopupMenu::highlightedBackgroundColourId, findColour(PhiColourIds::Module::Lowlight));
        setColour(juce::TextEditor::focusedOutlineColourId,       juce::Colours::transparentBlack);
        setColour(juce::TextEditor::highlightedTextColourId,      juce::Colour::greyLevel(0.7f));
        setColour(juce::ScrollBar::thumbColourId,                 juce::Colour::greyLevel(0.6f));
        setColour(juce::Slider::textBoxHighlightColourId,         juce::Colour::greyLevel(0.2f));
        setColour(juce::Slider::textBoxOutlineColourId,           juce::Colours::transparentBlack);
        setColour(juce::TooltipWindow::backgroundColourId,        juce::Colours::grey);
        setColour(juce::TooltipWindow::textColourId,              juce::Colours::darkgrey.darker());
        setColour(juce::ColourSelector::backgroundColourId,       juce::Colours::transparentBlack);
        setColour(juce::CaretComponent::caretColourId,            juce::Colour::greyLevel(0.8f));
        setColour(juce::Label::backgroundWhenEditingColourId,     juce::Colour::greyLevel(0.3f));
    }
    
    // This *must* be called after the lookandfeel has been set
    void setTheme(const PhiTheme& theme) {
        theme.apply(*this);
    }
    
    void drawCallOutBoxBackground (juce::CallOutBox& box, juce::Graphics& g, const juce::Path&, juce::Image&) override {
        auto bounds = box.getLocalBounds().reduced(15).toFloat();
        
        g.setColour (findColour(PhiColourIds::Module::Background));
        g.fillRoundedRectangle(bounds, 2.0f);
        
        g.setColour (findColour(PhiColourIds::Module::SelectedOutline));
        g.drawRoundedRectangle(bounds, 2.0f, 2.0f);
    };
    
    int getPopupMenuBorderSize() override { return 1; }
    
    void drawLasso(juce::Graphics& g, juce::Component& lasso) override {
        juce::Path path;
        path.addRectangle(lasso.getLocalBounds());
        
        const float dashes[2] = {5.0f, 5.0f};
        juce::PathStrokeType(2.0f).createDashedStroke(path, path, dashes, 2);
        
        g.setColour(juce::Colours::grey);
        g.fillPath(path);
    }
    
    void drawMenuBarBackground(juce::Graphics& g, int width, int height, bool isMouseOverBar, juce::MenuBarComponent& menu) override {
        
    }
    
    void drawMenuBarItem(juce::Graphics& g,
                         int width, int height,
                         int itemIndex, const juce::String &itemText,
                         bool isMouseOverItem,
                         bool isMenuOpen,
                         bool isMouseOverBar,
                         juce::MenuBarComponent& menu) override
    {
        // This function uses PopupMenu's colour IDs to maintain a consistent look
        g.setColour(menu.findColour(isMouseOverItem ? juce::PopupMenu::highlightedTextColourId : juce::PopupMenu::textColourId));
        g.drawText(itemText, juce::Rectangle<int>{0, 0, width, height}, juce::Justification::centred, 1);
    }
};
