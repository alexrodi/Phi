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
frequencyDial("Frequency", 20.0f, 20000.0f, 0.2f, " Hz"),
shapeDial("Shape", 0.0f, 1.0f, 1.0f, " %", 0, this),
frequencyAttachment(*processor.params.getParameter("freq"), frequencyDial),
shapeAttachment(*processor.params.getParameter("shape"), shapeDial)
{
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
    centreY = getBounds().toFloat().getCentreY();
    yRange = (float)getHeight() * 0.57f;
}

const void ImpulseUI::Waveform::setColour(const Colour& newStrokeColour, const Colour& newFillColour)
{
    strokeColour = newStrokeColour;
    fillColour = newFillColour;
}

void ImpulseUI::Waveform::paint(Graphics& g)
{
    g.setColour(fillColour);
    g.fillPath(path);
    
    g.setColour(strokeColour);
    g.strokePath (path, PathStrokeType (strokeWidth));
}


const void ImpulseUI::Waveform::updateForm(const float shape)
{
    const float shapeValue = powf(shape, 0.1f);
    
    const int pixelsPerPoint = 2;
    
    const float width           =  getWidth();
    const int   aaValue         =  8; // x8 AA
    const float aaRatio         =  1.0f / aaValue;
    // scale the values so that the waveform (more or less) fills the width
    const float phaseIncrement  =  ( ( ( powf( shapeValue, 50.0f ) * 200.0f + 30.0f) / width) * (float)pixelsPerPoint) * aaRatio;
    
    float phase = 0.0f;
    
    path.clear();
    path.startNewSubPath (strokeWidth, centreY);

    // Add lines to path
    for ( int x = strokeWidth; x < width; x += pixelsPerPoint )
    {
        float y = 0.0f;
        for( int i = 0; i < aaValue; i++ )
        {
            y += fabsf( ImpulseProcessor::processImpulse( phase, shapeValue ) );
            phase += phaseIncrement;
        }
        y *= aaRatio;
        path.lineTo ( x, y * yRange + centreY );
    }
    
    path.lineTo ( width, centreY );
    path = path.createPathWithRoundedCorners(60);
    
    auto bottomPath = path;
    bottomPath.applyTransform( AffineTransform().verticalFlip( (float)getHeight() ) );
    path.addPath(bottomPath);
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
    waveForm.setColour(findColour(Slider::thumbColourId), findColour(Slider::rotarySliderOutlineColourId));
}

void ImpulseUI::sliderValueChanged(Slider* slider)
{
    if (slider == &shapeDial)
    {
        waveForm.updateForm(slider->getValue());
        repaint();
    }
}
