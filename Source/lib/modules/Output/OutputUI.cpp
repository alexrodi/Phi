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
OutputUI::OutputUI(ModuleProcessor& processor) :
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
    auto height = moduleBounds.getHeight();
    auto width = moduleBounds.getWidth();
    
    bool svgVisible = jmin(height, width) > 30;
    
    speakerImage->setVisible(svgVisible);
    
    if (svgVisible)
    {
        auto bounds = getLocalBounds().withSizeKeepingCentre(100, 100).constrainedWithin(moduleBounds);
        
        speakerImage->setTransformToFit(bounds.toFloat(), 0);
    }
}

void OutputUI::lookAndFeelChanged()
{
    Colour newColour = findColour(Slider::thumbColourId);
    speakerImage->replaceColour(colour, newColour);
    colour = newColour;
}
