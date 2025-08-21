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

Plug::Plug(PlugType mode, const String& name) :
HoverPopupClient(this),
type(mode)
{
    setName(name);
    
    colors = mode == PlugType::Inlet
            ? PlugColors{Colours::grey, Colours::darkgrey}
            : PlugColors{Colours::darkgrey, Colours::grey};
    
    setPaintingIsUnclipped(true);
}

Plug::Plug(Plug&& other) noexcept :
Plug(other.type, other.getName())
{}

void Plug::setID(PlugID ioId)
{
    plugID = ioId;
}

void Plug::paint (Graphics& g)
{
    if (PlugOptions::drawName && canFitText) {
        g.setColour(Colours::grey);
        g.drawText(getName(), nameBounds, nameJustification);
        setHoverPopupEnabled(false);
    } else {
        setHoverPopupEnabled(true); // Display the tooltip if not showing text
    }
    
    g.setColour(colors.inner);
    g.fillEllipse(bounds);
    g.setColour(colors.outer);
    g.drawEllipse(bounds, STROKE_WIDTH);
}

void Plug::resized()
{
    nameJustification = Justification::Flags::centredBottom;
    nameBounds = getLocalBounds().withTrimmedBottom(getHeight() * 0.5 + 12);
    canFitText = nameBounds.getHeight() > 11;
    
    bounds = getLocalBounds().toFloat().withSizeKeepingCentre(SIZE, SIZE);
}

void Plug::mouseDown(const MouseEvent& e)
{
    emitEvent(PlugEvent(PlugEvent::StartConnection, e, type, plugID));
}

void Plug::mouseUp(const MouseEvent& e)
{
    auto topLevelComponent = getTopLevelComponent();
    auto mousePosition = topLevelComponent->getLocalPoint(this, e.position.toInt());
    auto componentUnderMouse = topLevelComponent->getComponentAt(mousePosition);
    
    if (auto plug = dynamic_cast<Plug*>(componentUnderMouse)) {
        emitEvent(PlugEvent(PlugEvent::EndConnection, e, plug->type, plug->plugID));
    } else {
        emitEvent(PlugEvent(PlugEvent::AbortConnection, e));
    }
}

void Plug::mouseDrag(const MouseEvent& e)
{
    emitEvent(PlugEvent(PlugEvent::Drag, e, type, plugID));
}

juce::Point<float> Plug::hoverPopupPosition() {
    return getLocalBounds().toFloat().getCentre().translated(0, -SIZE - 10);
}

juce::String Plug::getPopupText() { return getName(); }
