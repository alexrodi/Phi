/*
  ==============================================================================

    OutputUI.h
    Created: 5 Apr 2020 10:13:32pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

#include "../../ui/ModuleUI.h"

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
        .defaultSize = {130, 100},
        .minimumSize = {100, 100},
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
    
    void resized() override
    {
        auto bounds = getLocalBounds();
        auto speakerBounds = bounds.withSizeKeepingCentre(100, 100).constrainedWithin(bounds.reduced(10));
        
        speakerImage->setTransformToFit(speakerBounds.toFloat(), 0);
        
        speakerImage->setVisible(std::min(bounds.getHeight(), bounds.getWidth()) > 30);
    }

    void colourChanged() override
    {
        auto newColour = findColour(PhiColourIds::Module::Highlight);
        speakerImage->replaceColour(previousColour, newColour);
        previousColour = newColour;
    }

private:
    std::unique_ptr<juce::Drawable> speakerImage;
    juce::Colour previousColour;
};
