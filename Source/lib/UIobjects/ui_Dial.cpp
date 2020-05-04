/*
  ==============================================================================

    phi_Dial.cpp
    Created: 9 Feb 2020 11:02:38pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

///@cond
#include <JuceHeader.h>
///@endcond
#include "ui_Dial.h"

ui_Dial::ui_Dial(std::string name, float rangeLow, float rangeHigh, Slider::Listener* listener, double skewFactor, std::string valueSuffix, int decimals) :
valueRange{rangeLow, rangeHigh, 0, skewFactor}
{
    setNormalisableRange         (valueRange);
    setSliderStyle               (RotaryVerticalDrag);
    setTextBoxStyle              (TextBoxBelow, true, 80, 20);
    setTextBoxIsEditable         (true);
    
    setName                      (name);
    setTextValueSuffix           (valueSuffix);
    setNumDecimalPlacesToDisplay (decimals);
    addListener                  (listener);
    addListener (this);
    
    setPaintingIsUnclipped (true);
    
    updateDial();
}

 ui_Dial::~ui_Dial()
{
}


void ui_Dial::paint (Graphics& g)
{
    
    if (shouldDrawText)
    {
        g.setColour(nameColour);
        g.drawFittedText(getName(), nameBounds, Justification::centredBottom, 1);
    }

    g.setColour(grooveColour);
    g.strokePath(groove, PathStrokeType(thickness, PathStrokeType::JointStyle::mitered, PathStrokeType::EndCapStyle::rounded));
    
    g.setColour(colour);
    g.strokePath(dial, PathStrokeType(thickness, PathStrokeType::JointStyle::mitered, PathStrokeType::EndCapStyle::rounded));
    
    if (radius > 17)
    {
        g.addTransform(pointerRotation);
        g.fillPath(pointerPath);
        g.addTransform(pointerRotation.inverted());
    }

}

void ui_Dial::resized ()
{
    Slider::resized();
    
    bool hasName = getName() != "";
    bool enoughHeightForName = getHeight() > 70;
    
    shouldDrawText = hasName && enoughHeightForName;
    
    auto bounds = getLocalBounds();
    
    nameBounds = bounds.removeFromTop(15);
    
    size = jmin(bounds.getWidth(), bounds.getHeight()-10);
    box = bounds.toFloat().withSizeKeepingCentre(size, size).reduced(padding).translated(0, -7);
    radius = box.getWidth() * 0.55;
    
    groove.clear();
    groove.addCentredArc(box.getCentreX(), box.getCentreY(), radius, radius, 0, startAngle, endAngle, true);
    
    updateDial();
}

void ui_Dial::lookAndFeelChanged ()
{
    Slider::lookAndFeelChanged();
    colour = findColour(thumbColourId);
    grooveColour = findColour(rotarySliderOutlineColourId);
    nameColour = findColour(textBoxTextColourId);
}

void ui_Dial::updateDial ()
{
    angle = startAngle + valueRange.convertTo0to1(getValue()) * angleRange;
    
    dial.clear();
    dial.addCentredArc(box.getCentreX(), box.getCentreY(), radius, radius, 0, startAngle, angle, true);
    
    if (radius > 17)
    {
        pointerPath.clear();
        pointerPath.addRoundedRectangle(box.withSizeKeepingCentre(4, 10).withY(box.getY()+thickness), 2);
        pointerRotation = AffineTransform::rotation(angle, box.getCentreX(), box.getCentreY());
    }
}

void ui_Dial::sliderValueChanged (Slider* slider)
{
    if (slider == this) updateDial();
}
