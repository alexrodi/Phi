/*
  ==============================================================================

    Inlet.cpp
    Created: 18 Feb 2020 11:19:12pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

///@cond
#include <JuceHeader.h>
///@endcond
#include "Inlet.h"

//==============================================================================
namespace InletOptions
{
    bool drawName = true;
}

Inlet::Inlet(const String& name) :
name{name}
{
    setPaintingIsUnclipped(true);
}

Inlet::~Inlet()
{
}

void Inlet::setId(std::pair<uint32, int> inletId)
{
    moduleID = inletId.first;
    inletID = inletId.second;
}

String Inlet::getStringId()
{
    return String(moduleID) + ">" + String(inletID);
}

void Inlet::paint (Graphics& g)
{
    if (InletOptions::drawName && canFitText)
    {
        g.setColour(Colours::grey);
        g.drawFittedText(name, nameBounds, nameJustification, 3);
        setTooltip("");
    }
    else setTooltip(name); // Display the tooltip if not showing text
    
    g.setColour(Colours::grey);
    g.fillEllipse(inletBounds);
    g.setColour(Colours::darkgrey);
    g.drawEllipse(inletBounds, 3);
}

void Inlet::resized()
{
    nameJustification = Justification::Flags::centredBottom;
    nameBounds = getLocalBounds().withTrimmedBottom(getHeight() * 0.5 + 12);
    canFitText = nameBounds.getHeight() > 11;
    
    inletBounds = getLocalBounds().withSizeKeepingCentre(12, 12).toFloat();
}

void Inlet::mouseDown(const MouseEvent& e)
{
    sendActionMessage("mouseDown inlet #" + getStringId());
}

void Inlet::mouseUp(const MouseEvent& e)
{
    sendActionMessage("mouseUp");
}

void Inlet::mouseDrag(const MouseEvent& e)
{
    DragAndDropContainer::findParentDragContainerFor(this)->
    startDragging ("inlet" + getStringId()
                   , this
                   , Image (Image::PixelFormat::RGB, 1, 1, true));
    
    sendActionMessage("dragging");
}

bool Inlet::isInterestedInDragSource (const SourceDetails& dragSourceDetails)
{
    return dragSourceDetails.description.toString().contains("outlet");
}

void Inlet::itemDropped (const SourceDetails& dragSourceDetails)
{
    sendActionMessage("connect "
                      + getStringId() + "&"
                      + dragSourceDetails.description.toString().fromFirstOccurrenceOf("outlet", false, false));
}
