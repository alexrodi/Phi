/*
  ==============================================================================

    Outlet.cpp
    Created: 18 Feb 2020 11:19:27pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

///@cond
#include <JuceHeader.h>
///@endcond
#include "Outlet.h"

//==============================================================================
namespace OutletOptions
{
    bool drawName = true;
}

Outlet::Outlet(const String& name) :
name{name}
{
    setPaintingIsUnclipped(true);
}

Outlet::~Outlet()
{
}

void Outlet::setId(std::pair<uint32, int> outletId)
{
    moduleID = outletId.first;
    outletID = outletId.second;
}

String Outlet::getStringId()
{
    return String(moduleID) + ">" + String(outletID);
}

void Outlet::paint (Graphics& g)
{
    if (OutletOptions::drawName && canFitText)
    {
        g.setColour(Colours::grey);
        g.drawFittedText(name, nameBounds, nameJustification, 3);
        setTooltip("");
    }
    else setTooltip(name);
    
    g.setColour(Colours::darkgrey);
    g.fillEllipse(outletBounds);
    g.setColour(Colours::grey);
    g.drawEllipse(outletBounds, 3);
}

void Outlet::resized()
{
    nameJustification = Justification::Flags::centredBottom;
    nameBounds = getLocalBounds().withTrimmedBottom(getHeight() * 0.5 + 12);
    canFitText = nameBounds.getHeight() > 11;
    
    outletBounds = getLocalBounds().withSizeKeepingCentre(12, 12).toFloat();
}

void Outlet::mouseDown(const MouseEvent& e)
{
    sendActionMessage("mouseDown outlet #" + getStringId());
}

void Outlet::mouseUp(const MouseEvent& e)
{
    sendActionMessage("mouseUp");
}

void Outlet::mouseDrag(const MouseEvent& e)
{
    DragAndDropContainer::findParentDragContainerFor(this)->
    startDragging ("outlet" + getStringId()
                   , this
                   , Image (Image::PixelFormat::RGB, 1, 1, true));
    
    sendActionMessage("dragging");
}

bool Outlet::isInterestedInDragSource (const SourceDetails& dragSourceDetails)
{
    return dragSourceDetails.description.toString().contains("inlet");
}

void Outlet::itemDropped (const SourceDetails& dragSourceDetails)
{
    sendActionMessage("connect "
                      + dragSourceDetails.description.toString().fromFirstOccurrenceOf("inlet", false, false)
                      + "&"
                      + getStringId());
}
