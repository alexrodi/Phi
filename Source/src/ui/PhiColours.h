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
        General = 0x000,
        Module = 0x100,
        Connection = 0x200,
        Port = 0x300,
    };
public:
    struct General {enum Val{
       Background = (int)ColourClass::General,
       TopBar
    };};

    struct Module {enum Val{
        Background = (int)ColourClass::Module,
        Outline,
        Text,
        SelectedOutline,
        SelectedText,
        HeaderLine,
        Highlight
    };};

    struct Connection {enum Val{
        DefaultFill = (int)ColourClass::Connection,
        SelectedOutline
    };};
    
    struct Port {enum Val{
        IntletOutline = (int)ColourClass::Port,
        IntletFill,
        OutletOutline,
        OutletFill,
        Text
    };};
};

struct PhiLookAndFeel  : public juce::LookAndFeel_V4
{
    PhiLookAndFeel()
    {
        setColour(juce::PopupMenu::textColourId,                  juce::Colours::lightgrey);
        setColour(juce::PopupMenu::backgroundColourId,            juce::Colours::darkgrey.darker());
        setColour(juce::PopupMenu::highlightedBackgroundColourId, juce::Colour::greyLevel(0.2));
        
        setColour(juce::TextButton::textColourOnId, juce::Colours::grey.brighter());
        setColour(juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
        
        setColour(juce::TextEditor::focusedOutlineColourId,  juce::Colour()); // no color
        setColour(juce::TextEditor::highlightedTextColourId, juce:: Colour::greyLevel(0.7f));
        
        setColour(juce::ScrollBar::thumbColourId, juce::Colours::lightgrey.withAlpha(0.5f));
        
        setColour(juce::Slider::thumbColourId,               juce::Colour::greyLevel(0.8f));
        setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour::greyLevel(0.21f));
        setColour(juce::Slider::rotarySliderFillColourId,    juce::Colour::greyLevel(0.17f));
        setColour(juce::Slider::textBoxHighlightColourId,    juce::Colour::greyLevel(0.2f));
        setColour(juce::Slider::textBoxTextColourId,         juce::Colours::grey.brighter());
        setColour(juce::Slider::textBoxOutlineColourId,      juce::Colour()); // no color
        
        setColour(juce::TooltipWindow::backgroundColourId, juce::Colours::grey);
        setColour(juce::TooltipWindow::textColourId,       juce::Colours::darkgrey.darker());
        
        setColour(juce::ColourSelector::backgroundColourId, juce::Colours::transparentBlack);
        
        setColour(juce::CaretComponent::caretColourId,        juce::Colour::greyLevel(0.8f));
        
        setColour(juce::Label::backgroundWhenEditingColourId, juce::Colour::greyLevel(0.3f));
        
        
        // General
        setColour(PhiColourIds::General::Background, juce::Colours::darkgrey);
        setColour(PhiColourIds::General::TopBar,     juce::Colours::darkgrey.darker());
        
        // Modules (defaults)
        setColour(PhiColourIds::Module::Highlight,       juce::Colours::cyan.withSaturation(0.5f));
        setColour(PhiColourIds::Module::Background,      juce::Colours::darkgrey.darker());
        setColour(PhiColourIds::Module::Outline,         juce::Colours::grey);
        setColour(PhiColourIds::Module::Text,            juce::Colours::grey);
        setColour(PhiColourIds::Module::SelectedOutline, juce::Colours::grey.brighter());
        setColour(PhiColourIds::Module::SelectedText,    juce::Colours::grey.brighter());
        setColour(PhiColourIds::Module::HeaderLine,      juce::Colours::grey);
        
        // Ports
        setColour(PhiColourIds::Port::IntletOutline, juce::Colours::grey);
        setColour(PhiColourIds::Port::IntletFill,    juce::Colours::darkgrey);
        setColour(PhiColourIds::Port::OutletOutline, juce::Colours::darkgrey);
        setColour(PhiColourIds::Port::OutletFill,    juce::Colours::grey);
        setColour(PhiColourIds::Port::Text,          juce::Colours::grey);
        
        // Connections
        setColour(PhiColourIds::Connection::DefaultFill,     juce::Colours::grey);
        setColour(PhiColourIds::Connection::SelectedOutline, juce::Colours::grey.brighter());
        
        setDefaultSansSerifTypefaceName ("Helvetica Neue");
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
};

inline void openColourSelector(juce::Rectangle<int> area, juce::Colour initialColour, juce::Component* comp, juce::ChangeListener* listener)
{
    auto colourSelector = std::make_unique<juce::ColourSelector>(juce::ColourSelector::showColourspace);
    colourSelector->setSize(150, 130);
    colourSelector->addChangeListener(listener);
    colourSelector->setCurrentColour(initialColour);

    auto& callOutBox = juce::CallOutBox::launchAsynchronously(std::move(colourSelector), area, comp);
    
    callOutBox.setLookAndFeel(&comp->getLookAndFeel());
    callOutBox.setAlwaysOnTop(true);
}
