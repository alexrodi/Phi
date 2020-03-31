/*
  ==============================================================================

    phi_Dial.cpp
    Created: 9 Feb 2020 11:02:38pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ui_Dial.h"

ui_Dial::ui_Dial(float rangeLow, float rangeHigh, std::string valueSuffix, int decimals, Slider::Listener* listener)
{
    setRange                     (rangeLow, rangeHigh);
    setTextValueSuffix           (valueSuffix);
    addListener                  (listener);
    setSliderStyle               (Slider::RotaryVerticalDrag);
    setTextBoxStyle              (Slider::TextBoxBelow, true, 80, 20);
    setNumDecimalPlacesToDisplay (decimals);
    setTextBoxIsEditable         (true);
    
    setPaintingIsUnclipped (true);
}

 ui_Dial::~ui_Dial()
{
}

void ui_Dial::paint (Graphics& g)
{
    const float padding = 10;
    const float thickness = 5;
    const float startAngle = getRotaryParameters().startAngleRadians;
    const float endAngle = getRotaryParameters().endAngleRadians;
    const float range = endAngle - startAngle;
    const float size = jmin(getWidth(), getHeight()-10);
    Rectangle<float> box = getLocalBounds().toFloat().withSizeKeepingCentre(size, size).reduced(padding).translated(0, -5);
    const float radius = box.getWidth() * 0.5;
    
    Path groove;
    
    groove.addCentredArc(box.getCentreX(), box.getCentreY(), radius, radius, 0, startAngle, endAngle, true);
    
    g.setColour(findColour(Slider::rotarySliderOutlineColourId));
    g.strokePath(groove, PathStrokeType(thickness));
    
    Path dial;
    
    const float angle = startAngle + (getValue()-getMinimum())/getRange().getLength() * range;
    
    dial.addCentredArc(box.getCentreX(), box.getCentreY(), radius, radius, 0, startAngle, angle, true);
    
    g.setColour(findColour(Slider::thumbColourId));
    g.strokePath(dial, PathStrokeType(thickness, PathStrokeType::JointStyle::mitered, PathStrokeType::EndCapStyle::rounded));
    
    if (radius > 18)
    {
        Path pointerPath;
        
        pointerPath.addRoundedRectangle(box.withSizeKeepingCentre(4, 10).withY(box.getY()+thickness+2), 2);
        
        AffineTransform rotation = AffineTransform::rotation(angle, box.getCentreX(), box.getCentreY());
        
        g.addTransform(rotation);
        g.fillPath(pointerPath);
        g.addTransform(rotation.inverted());
    }

}
