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

ui_Dial::ui_Dial(float rangeLow, float rangeHigh, std::string valueSuffix, int decimals, Slider::Listener* listener)
{
    setRange                     (rangeLow, rangeHigh);
    setTextValueSuffix           (valueSuffix);
    addListener                  (listener);
    setSliderStyle               (RotaryVerticalDrag);
    setTextBoxStyle              (TextBoxBelow, true, 80, 20);
    setNumDecimalPlacesToDisplay (decimals);
    setTextBoxIsEditable         (true);
    
    setPaintingIsUnclipped (true);
    
    addListener(this);
    updateDial();
}

 ui_Dial::~ui_Dial()
{
}


void ui_Dial::paint (Graphics& g)
{

    g.setColour(grooveColour);
    g.strokePath(groove, PathStrokeType(thickness));
    
    g.setColour(colour);
    g.strokePath(dial, PathStrokeType(thickness, PathStrokeType::JointStyle::mitered, PathStrokeType::EndCapStyle::rounded));
    
    if (radius > 18)
    {
        g.addTransform(pointerRotation);
        g.fillPath(pointerPath);
        g.addTransform(pointerRotation.inverted());
    }

}

void ui_Dial::resized ()
{
    Slider::resized();
    
    size = jmin(getWidth(), getHeight()-10);
    box = getLocalBounds().toFloat().withSizeKeepingCentre(size, size).reduced(padding).translated(0, -5);
    radius = box.getWidth() * 0.5;
    
    groove.clear();
    groove.addCentredArc(box.getCentreX(), box.getCentreY(), radius, radius, 0, startAngle, endAngle, true);
    
    updateDial();
}

void ui_Dial::lookAndFeelChanged ()
{
    Slider::lookAndFeelChanged();
    colour = findColour(thumbColourId);
    grooveColour = findColour(rotarySliderOutlineColourId);
}

void ui_Dial::updateDial ()
{
    angle = startAngle + (getValue()-getMinimum())/getRange().getLength() * range;
    
    dial.clear();
    dial.addCentredArc(box.getCentreX(), box.getCentreY(), radius, radius, 0, startAngle, angle, true);
    
    if (radius > 18)
    {
        pointerPath.clear();
        
        pointerPath.addRoundedRectangle(box.withSizeKeepingCentre(4, 10).withY(box.getY()+thickness+2), 2);
        
        pointerRotation = AffineTransform::rotation(angle, box.getCentreX(), box.getCentreY());
    }
}

void ui_Dial::sliderValueChanged (Slider* slider)
{
    if (slider == this) updateDial();
}
