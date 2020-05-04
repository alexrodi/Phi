/*
  ==============================================================================

    Impulse.cpp
    Created: 9 Feb 2020 11:02:38pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

///@cond
#include <JuceHeader.h>
///@endcond
#include "ImpulseUI.h"

//==============================================================================
ImpulseUI::ImpulseUI(ImpulseProcessor& processor) :
ModuleUI{{
    // All modules must initialize these properties
    .name =  "Impulse",
    .inlets = {"Trigger", "Freq", "Shape"},
    .outlets = {"Out", "Ramp"},
    .width = 370,
    .height = 220,
    .minimumHeight = 100,
    .processor = processor
}},
processor(processor),
frequencyDial("Frequency", 20, 20000, this, 0.2, " Hz"),
shapeDial("Shape", 0, 1, this, 1, " %", 0)
{
    shapeDial.textFromValueFunction = [] (float f) -> String { return String(int(f * 100)); };
    shapeDial.valueFromTextFunction = [] (String s) -> float { return float(s.toUTF8().getDoubleValue()) * 0.01; };
    
    waveForm.setInterceptsMouseClicks(false, false);

    addAndMakeVisible(waveForm);
    addAndMakeVisible(frequencyDial);
    addAndMakeVisible(shapeDial);
}

ImpulseUI::~ImpulseUI()
{
}

// Waveform
//==============================================================================

void ImpulseUI::mouseDown(const MouseEvent& e)
{
    processor.triggerImpulse();
}

void ImpulseUI::Waveform::resized()
{
    centreY = getBounds().getCentreY();
    yRange = getHeight() * 0.5;
    updateColour();
}

const void ImpulseUI::Waveform::setColour(const Colour& colourToUse)
{
    colour = colourToUse;
    updateColour();
}

const void ImpulseUI::Waveform::updateColour()
{
    colourGradient = ColourGradient().vertical(colour, centreY+yRange, colour.darker(), centreY);
}

void ImpulseUI::Waveform::paint(Graphics& g)
{
    g.setGradientFill(colourGradient);
    g.fillPath(topPath);
    g.strokePath (topPath, PathStrokeType (strokeWidth));
    
    g.setColour(colour);
    g.fillPath(bottomPath);
    g.strokePath (bottomPath, PathStrokeType (strokeWidth));
}


const void ImpulseUI::Waveform::updateForm(const float shape)
{
    const float shapeValue = powf(shape, 0.1f);
    
    const int pixelsPerPoint = 2;
    
    const float width           =  getWidth();
    const int   aaValue         =  8; // x8 AA
    // scale the values so that the waveform (more or less) fills the width
    const float phaseIncrement  =  (((powf(shapeValue,50)*200 + 30)/width)*pixelsPerPoint)/aaValue;
    
    float phase = 0;
    
    topPath.clear();
    topPath.startNewSubPath (0, centreY);

    // Add lines to path
    for (int x=0; x<width; x += pixelsPerPoint){
        float y = 0;
        for(int i=0; i<aaValue; i++)
        {
            y += fabs(ImpulseProcessor::processImpulse(phase, shapeValue));
            phase += phaseIncrement;
        }
        y /= aaValue;
        topPath.lineTo (x, y * yRange + centreY);
    }
    
    topPath.lineTo (width, centreY);
    topPath = topPath.createPathWithRoundedCorners(60);
    
    bottomPath = topPath;
    bottomPath.applyTransform(AffineTransform().verticalFlip(centreY+yRange));
}

//==============================================================================

void ImpulseUI::paint (Graphics& g)
{
}

void ImpulseUI::wasResized(Rectangle<int> moduleBounds)
{
    // Place the Dials
    Rectangle<int> dialBounds = moduleBounds.removeFromLeft(getWidth()*0.25);
    frequencyDial.setBounds( dialBounds.removeFromTop(getHeight()*0.48));
    shapeDial.setBounds( dialBounds.removeFromBottom(getHeight()*0.48));
    
    // Place the waveform and update
    waveForm.setBounds(moduleBounds.reduced(10,0));
    waveForm.updateForm(shapeDial.getValue());
}

void ImpulseUI::lookAndFeelChanged()
{
    waveForm.setColour(findColour(Slider::thumbColourId));
}

void ImpulseUI::frequencyDialChanged (float value)
{
    *processor.params.getRawParameterValue("freq") = value;
}

void ImpulseUI::shapeDialChanged (float value)
{
    *processor.params.getRawParameterValue("shape") = value;
    waveForm.updateForm(shapeDial.getValue());
    repaint();
}

void ImpulseUI::sliderValueChanged (Slider* slider)
{
    if (slider == &frequencyDial)
        frequencyDialChanged(slider->getValue());
    else if (slider == &shapeDial)
        shapeDialChanged(slider->getValue());
}
