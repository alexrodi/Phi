/*
  ==============================================================================

    OutputUI.h
    Created: 5 Apr 2020 10:13:32pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

///@cond
#include <JuceHeader.h>
///@endcond
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
    speakerImage(Drawable::createFromSVG(*XmlDocument::parse(BinaryData::Speaker_Icon_svg))),
    colour(findColour(Slider::thumbColourId))
    {
        speakerImage->replaceColour(Colours::black, colour);
        addAndMakeVisible (*speakerImage);
    }
    
    ~OutputUI() {}

    void paint (Graphics&) override {}
    void onResize(Rectangle<int> moduleBounds) override
    {
        auto bounds = moduleBounds.withSizeKeepingCentre(100, 100).constrainedWithin(moduleBounds.reduced(10));
        
        speakerImage->setTransformToFit(bounds.toFloat(), 0);
        
        speakerImage->setVisible(std::min(moduleBounds.getHeight(), moduleBounds.getWidth()) > 30);
    }

    void lookAndFeelChanged() override
    {
        Colour newColour = findColour(Slider::thumbColourId);
        speakerImage->replaceColour(colour, newColour);
        colour = newColour;
    }

private:
    std::unique_ptr<Drawable> speakerImage;

    Colour colour;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OutputUI)
};
