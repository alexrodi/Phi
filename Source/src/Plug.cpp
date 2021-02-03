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

Plug::Plug(PlugMode modeToUse, const String& nameToUse) :
name(nameToUse),
mode(modeToUse)
{
    colors = mode == PlugMode::Inlet
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
    emitEvent(PlugMouseEvent(mode, plugID, e, PlugMouseEvent::Type::MouseDown));
}

void Plug::mouseUp(const MouseEvent& e)
{
    auto topLevelComponent = getTopLevelComponent();
    auto mousePosition = topLevelComponent->getLocalPoint(this, e.position.toInt());
    auto componentUnderMouse = topLevelComponent->getComponentAt(mousePosition);
    
    if (auto plug = dynamic_cast<Plug*>(componentUnderMouse)) {
        emitEvent(PlugMouseEvent(plug->mode, plug->plugID, e, PlugMouseEvent::Type::MouseUp));
    } else {
        emitEvent(PlugEndDragEvent{});
    }
}

void Plug::mouseDrag(const MouseEvent& e)
{
    emitEvent(PlugMouseEvent(mode, plugID, e, PlugMouseEvent::Type::MouseDrag));
}
