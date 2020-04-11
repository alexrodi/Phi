/*
  ==============================================================================

    phi_Inlet.cpp
    Created: 18 Feb 2020 11:19:12pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

///@cond
#include <JuceHeader.h>
///@endcond
#include "phi_Inlet.h"

//==============================================================================
phi_Inlet::phi_Inlet(String name, NamePosition namePosition) :
namePosition{namePosition},
name{name}
{
    setPaintingIsUnclipped(true);
}

phi_Inlet::~phi_Inlet()
{
}

void phi_Inlet::setId(std::pair<uint32, int> inletId)
{
    moduleID = inletId.first;
    inletID = inletId.second;
}

String phi_Inlet::getStringId()
{
    return String(moduleID) + ">" + String(inletID);
}

void phi_Inlet::paint (Graphics& g)
{
    if (drawText)
    {
        g.setColour(Colours::grey);
        g.drawFittedText(name, nameBounds, nameJustification, 3);
    }
    
    g.setColour(Colours::grey);
    g.fillEllipse(inletBounds);
    g.setColour(Colours::darkgrey);
    g.drawEllipse(inletBounds, 3);
}

void phi_Inlet::resized()
{
    switch (namePosition) {
        case NamePosition::Left :
            nameJustification = Justification::Flags::centredRight;
            nameBounds = getLocalBounds().withTrimmedRight(getWidth() * 0.5 + 12);
            break;
        case NamePosition::Right :
            nameJustification = Justification::Flags::centredLeft;
            nameBounds = getLocalBounds().withTrimmedLeft(getWidth() * 0.5 + 12);
            break;
        case NamePosition::Above :
            nameJustification = Justification::Flags::centredBottom;
            nameBounds = getLocalBounds().withTrimmedBottom(getHeight() * 0.5 + 12);
            break;
        case NamePosition::Below :
            nameJustification = Justification::Flags::centredTop;
            nameBounds = getLocalBounds().withTrimmedTop(getHeight() * 0.5 + 12);
            break;
    }
    
    drawText = nameBounds.getHeight() > 11;
    
    inletBounds = getLocalBounds().withSizeKeepingCentre(12, 12).toFloat();
}

void phi_Inlet::mouseDown(const MouseEvent& e)
{
    sendActionMessage("mouseDown inlet #" + getStringId());
}

void phi_Inlet::mouseUp(const MouseEvent& e)
{
    sendActionMessage("mouseUp");
}

void phi_Inlet::mouseDrag(const MouseEvent& e)
{
    DragAndDropContainer::findParentDragContainerFor(this)->
    startDragging ("inlet" + getStringId()
                   , this
                   , Image (Image::PixelFormat::RGB, 1, 1, true));
    
    sendActionMessage("dragging");
}

bool phi_Inlet::isInterestedInDragSource (const SourceDetails& dragSourceDetails)
{
    return dragSourceDetails.description.toString().contains("outlet");
}

void phi_Inlet::itemDropped (const SourceDetails& dragSourceDetails)
{
    sendActionMessage("connect "
                      + getStringId() + "&"
                      + dragSourceDetails.description.toString().fromFirstOccurrenceOf("outlet", false, false));
}
