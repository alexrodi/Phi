/*
  ==============================================================================

    PhiDial.cpp
    Created: 9 Feb 2020 11:02:38pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#include "PhiDial.h"
#include "../PhiColours.h"

PhiDial::PhiDial()
{
    setSliderStyle(RotaryVerticalDrag);
    setTextBoxStyle(TextBoxBelow, false, 80, 20);
    setScrollWheelEnabled(false);
    
    addListener(this);
    
    setPaintingIsUnclipped(true);
    
    updateDial();
    
}

PhiDial::PhiDial(juce::RangedAudioParameter& parameter) :
PhiDial()
{
    attachment = std::make_unique<juce::SliderParameterAttachment>(parameter, *this);
    setName(parameter.getName(99));
}

 PhiDial::~PhiDial()
{
}


void PhiDial::paint (juce::Graphics& g)
{
    g.setColour(nameColour);
    g.drawFittedText(getName(), nameBounds, juce::Justification::centredBottom, 1);

    g.setColour(grooveColour);
    g.strokePath(groove, stroke);
    
    g.setColour(colour);
    g.strokePath(dial, stroke);
    
    if (radius > 17)
    {
        g.addTransform(pointerRotation);
        g.fillPath(pointerPath);
        g.addTransform(pointerRotation.inverted());
    }

}

void PhiDial::resized ()
{
    Slider::resized();
    
    if (getHeight() < 70)
        setTextBoxStyle(NoTextBox, true, 0, 0);
    else
        setTextBoxStyle(TextBoxBelow, false, 80, 20);
    
    auto bounds = getLocalBounds().withTrimmedBottom(getTextBoxHeight() - 7);
    
    nameBounds = bounds.removeFromTop(15);
    
    float size = std::min(bounds.getWidth(), bounds.getHeight());
    size = std::min(100.0f, size);
    
    dialBounds = bounds.toFloat().withSizeKeepingCentre(size, size).reduced(padding);
    
    radius = dialBounds.getWidth() * 0.55f;
    
    groove.clear();
    groove.addCentredArc(dialBounds.getCentreX(), dialBounds.getCentreY(), radius, radius, 0, startAngle, endAngle, true);
    
    updateDial();
}

void PhiDial::colourChanged ()
{
    Slider::colourChanged();
    colour = findColour(PhiColourIds::Module::Highlight);
    grooveColour = findColour(PhiColourIds::Module::Lowlight);
    nameColour = findColour(PhiColourIds::Module::Text);
    setColour(juce::Slider::textBoxTextColourId, nameColour);
}

void PhiDial::updateDial ()
{
    angle = startAngle + valueToProportionOfLength(getValue()) * angleRange;
    
    dial.clear();
    dial.addCentredArc(dialBounds.getCentreX(), dialBounds.getCentreY(), radius, radius, 0, startAngle, angle, true);
    
    if (radius > 17)
    {
        pointerPath.clear();
        pointerPath.addRoundedRectangle(dialBounds.withSizeKeepingCentre(thickness, 9).withY(dialBounds.getY()+thickness+1), thickness * 0.5);
        pointerRotation = juce::AffineTransform::rotation(angle, dialBounds.getCentreX(), dialBounds.getCentreY());
    }
}

void PhiDial::sliderValueChanged (Slider* slider)
{
    if (slider == this) updateDial();
}
