/*
  ==============================================================================

    OutputUI.h
    Created: 5 Apr 2020 10:13:32pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

#include "../../ModuleProcessor.h"

//==============================================================================
/*
*/
class OutputUI    : public ModuleUI
{
public:
    OutputUI(ModuleProcessor& processor) :
    ModuleUI{{
        // All modules must initialize these properties
        .name =  "Output",
        .inlets = {"L", "R"},
        .outlets = {}, // outlets are hidden because this is an output module
        .width = 150,
        .height = 150,
        .minimumHeight = 100,
        .processor = processor
    }},
    speakerImage(juce::Drawable::createFromSVG(*juce::XmlDocument::parse(BinaryData::Speaker_Icon_svg))),
    previousColour(findColour(juce::Slider::thumbColourId))
    {
        speakerImage->replaceColour(juce::Colours::black, previousColour);
        addAndMakeVisible (*speakerImage);
    }
    
    ~OutputUI() {}

    void paint (juce::Graphics&) override {}
    void onResize(juce::Rectangle<int> moduleBounds) override
    {
        auto bounds = moduleBounds.withSizeKeepingCentre(100, 100).constrainedWithin(moduleBounds.reduced(10));
        
        speakerImage->setTransformToFit(bounds.toFloat(), 0);
        
        speakerImage->setVisible(std::min(moduleBounds.getHeight(), moduleBounds.getWidth()) > 30);
    }

    void lookAndFeelChanged() override
    {
        auto newColour = findColour(juce::Slider::thumbColourId);
        speakerImage->replaceColour(previousColour, newColour);
        previousColour = newColour;
    }

private:
    std::unique_ptr<juce::Drawable> speakerImage;
    juce::Colour previousColour;
};
