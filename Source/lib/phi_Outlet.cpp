/*
  ==============================================================================

    phi_Outlet.cpp
    Created: 18 Feb 2020 11:19:27pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

///@cond
#include <JuceHeader.h>
///@endcond
#include "phi_Outlet.h"

//==============================================================================
namespace OutletOptions
{
    bool drawName = true;
}

phi_Outlet::phi_Outlet(const String& name) :
name{name}
{
    setPaintingIsUnclipped(true);
}

phi_Outlet::~phi_Outlet()
{
}

void phi_Outlet::setId(std::pair<uint32, int> outletId)
{
    moduleID = outletId.first;
    outletID = outletId.second;
}

String phi_Outlet::getStringId()
{
    return String(moduleID) + ">" + String(outletID);
}

void phi_Outlet::paint (Graphics& g)
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

void phi_Outlet::resized()
{
    nameJustification = Justification::Flags::centredBottom;
    nameBounds = getLocalBounds().withTrimmedBottom(getHeight() * 0.5 + 12);
    canFitText = nameBounds.getHeight() > 11;
    
    outletBounds = getLocalBounds().withSizeKeepingCentre(12, 12).toFloat();
}

void phi_Outlet::mouseDown(const MouseEvent& e)
{
    sendActionMessage("mouseDown outlet #" + getStringId());
}

void phi_Outlet::mouseUp(const MouseEvent& e)
{
    sendActionMessage("mouseUp");
}

void phi_Outlet::mouseDrag(const MouseEvent& e)
{
    DragAndDropContainer::findParentDragContainerFor(this)->
    startDragging ("outlet" + getStringId()
                   , this
                   , Image (Image::PixelFormat::RGB, 1, 1, true));
    
    sendActionMessage("dragging");
}

bool phi_Outlet::isInterestedInDragSource (const SourceDetails& dragSourceDetails)
{
    return dragSourceDetails.description.toString().contains("inlet");
}

void phi_Outlet::itemDropped (const SourceDetails& dragSourceDetails)
{
    sendActionMessage("connect "
                      + dragSourceDetails.description.toString().fromFirstOccurrenceOf("inlet", false, false)
                      + "&"
                      + getStringId());
}
