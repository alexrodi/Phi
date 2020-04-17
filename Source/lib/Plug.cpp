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
mode{modeToUse},
name{nameToUse}
{
    colors = mode==Inlet
             ? ColourPair(Colours::grey, Colours::darkgrey)
             : ColourPair(Colours::darkgrey, Colours::grey);
    
    modeString = mode==Inlet
                 ? StringPair("inlet", "outlet")
                 : StringPair("outlet", "inlet");
    
    setPaintingIsUnclipped(true);
}

Plug::~Plug()
{
}

void Plug::setId(std::pair<uint32, int> ioId)
{
    moduleID = ioId.first;
    ownID = ioId.second;
}

String Plug::getStringId()
{
    return String(moduleID) + ">" + String(ownID);
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
    sendActionMessage("mouseDown " + modeString.first + " #" + getStringId());
}

void Plug::mouseUp(const MouseEvent& e)
{
    sendActionMessage("mouseUp");
}

void Plug::mouseDrag(const MouseEvent& e)
{
    DragAndDropContainer::findParentDragContainerFor(this)->
    startDragging (modeString.first + getStringId()
                   , this
                   , Image (Image::PixelFormat::RGB, 1, 1, true));
    
    sendActionMessage("dragging");
}

bool Plug::isInterestedInDragSource (const SourceDetails& dragSourceDetails)
{
    String sourceString =  dragSourceDetails.description.toString();
    if (sourceString.startsWith(modeString.second))
    {
        uint32 receivedModuleID = sourceString.fromFirstOccurrenceOf(modeString.second, false, false)
                                                     .upToFirstOccurrenceOf(">", false, false)
                                                     .toUTF8().getIntValue32();
        
        // Not interested if the connection is from the same module
        if (receivedModuleID != moduleID)
        {
            return true;
        }
    }
    return false;
}

void Plug::itemDropped (const SourceDetails& dragSourceDetails)
{
    
    StringPair connectionIdString;
    
    String receivedId = dragSourceDetails.description.toString().fromFirstOccurrenceOf(modeString.second, false, false);
    
    if (mode==Inlet)
        connectionIdString = StringPair(getStringId(), receivedId);
    else if (mode==Outlet)
        connectionIdString = StringPair(receivedId, getStringId());
    
    sendActionMessage("connect "
                      + connectionIdString.first
                      + "&"
                      + connectionIdString.second);
}

