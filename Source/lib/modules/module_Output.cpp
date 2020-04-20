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
#include "module_Output.h"

//==============================================================================
module_Output::module_Output() :
Module{{
    // All modules must initialize these properties
    .name =  "Output",
    .inlets = {"R", "L"},
    .outlets = {},
    .width = 150,
    .height = 150,
    .minimumHeight = 100
}},
speakerImage(Drawable::createFromSVG(*XmlDocument::parse(BinaryData::Speaker_Icon_svg))),
colour(findColour(Slider::thumbColourId))
{
    // module_Output cheats on the number of output channels
    // these are only used internally for audio and are not displayed
    setPlayConfigDetails (inletNumber, inletNumber, getSampleRate(), getBlockSize());
    
    speakerImage->replaceColour(Colours::black, colour);
    
    addAndMakeVisible (*speakerImage);
}

module_Output::~module_Output()
{
}

void module_Output::paint (Graphics& g)
{
}

void module_Output::wasResized(Rectangle<int> moduleBounds)
{
    Rectangle<int> svgBounds = moduleBounds.reduced(10, 25);
    speakerImage->setTransformToFit(svgBounds.withSizeKeepingCentre(100, 100).constrainedWithin(svgBounds).toFloat(), RectanglePlacement::Flags::centred);
}

void module_Output::lookAndFeelChanged()
{
    Colour newColour = findColour(Slider::thumbColourId);
    speakerImage->replaceColour(colour, newColour);
    colour = newColour;
}

//==============================================================================

void module_Output::prepareToPlay (double sampleRate, int maximumExpectedSamplesPerBlock)
{
}

void module_Output::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
}

void module_Output::releaseResources()
{
}
