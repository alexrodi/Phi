/*
  ==============================================================================

    Plug.cpp
    Created: 16 Apr 2020 11:08:52pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#include <JuceHeader.h>
#include "Plug.h"

//==============================================================================
namespace PlugOptions
{
    bool drawName = false;
}

Plug::Plug(Mode modeToUse, const String& nameToUse) :
name(nameToUse),
mode(modeToUse)
{
    colors = mode==Inlet
             ? ColourPair(Colours::grey, Colours::darkgrey)
             : ColourPair(Colours::darkgrey, Colours::grey);
    
    setPaintingIsUnclipped(true);
}

void Plug::setId(uint64 ioId)
{
    plugID = ioId;
}

void Plug::paint (Graphics& g)
{
    if (PlugOptions::drawName && canFitText)
    {
        g.setColour(Colours::grey);
        g.drawFittedText(name, nameBounds, nameJustification, 3);
        setTooltip("");
    }
    else setTooltip(name); // Display the tooltip if not showing text
    
    g.setColour(colors.first);
    g.fillEllipse(bounds);
    g.setColour(colors.second);
    g.drawEllipse(bounds, 3);
}

void Plug::resized()
{
    nameJustification = Justification::Flags::centredBottom;
    nameBounds = getLocalBounds().withTrimmedBottom(getHeight() * 0.5 + 12);
    canFitText = nameBounds.getHeight() > 11;
    
    bounds = getLocalBounds().withSizeKeepingCentre(12, 12).toFloat();
}

void Plug::mouseDown(const MouseEvent& e)
{
    emitEvent(MouseDown(mode, plugID));
}

void Plug::mouseUp(const MouseEvent& e)
{
    emitEvent(MouseUp());
}

void Plug::mouseDrag(const MouseEvent& e)
{
    DragAndDropContainer::findParentDragContainerFor(this)->
    startDragging (0
                   , this
                   , Image (Image::PixelFormat::RGB, 1, 1, true));
    
    emitEvent(Drag());
}

bool Plug::isInterestedInDragSource (const SourceDetails& dragSourceDetails)
{
    auto dragged = static_cast<Plug*>(dragSourceDetails.sourceComponent.get());
    
    // Only interested if the connection is of opposite mode and from a different module
    return (dragged->getMode() == getOppositeMode()) && (dragged->getID().moduleID() != plugID.moduleID());
}

void Plug::itemDropped (const SourceDetails& dragSourceDetails)
{
    auto dragged = static_cast<Plug*>(dragSourceDetails.sourceComponent.get());
    
    PlugID source;
    PlugID destination;
    
    if (getMode() == Mode::Inlet) {
        source = dragged->getID();
        destination = plugID;
    } else {
        source = plugID;
        destination = dragged->getID();
    }
    emitEvent(Connect(source, destination));
}
