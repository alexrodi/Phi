/*
  ==============================================================================

    module_Output.cpp
    Created: 5 Apr 2020 10:13:32pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

///@cond
#include <JuceHeader.h>
///@endcond
#include "OutputUI.h"

//==============================================================================
OutputUI::OutputUI(OutputProcessor& processor) :
ModuleUI{{
    // All modules must initialize these properties
    .name =  "Output",
    .inlets = {"R", "L"},
    .outlets = {},
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

OutputUI::~OutputUI()
{
}

void OutputUI::paint (Graphics& g)
{
}

void OutputUI::wasResized(Rectangle<int> moduleBounds)
{
    Rectangle<int> svgBounds = moduleBounds.reduced(10, 25);
    speakerImage->setTransformToFit(svgBounds.withSizeKeepingCentre(100, 100).constrainedWithin(svgBounds).toFloat(), RectanglePlacement::Flags::centred);
}

void OutputUI::lookAndFeelChanged()
{
    Colour newColour = findColour(Slider::thumbColourId);
    speakerImage->replaceColour(colour, newColour);
    colour = newColour;
}
