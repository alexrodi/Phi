/*
  ==============================================================================

    Impulse.cpp
    Created: 9 Feb 2020 11:02:38pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#include <JuceHeader.h>
#include "module_Impulse.h"

//==============================================================================
module_Impulse::module_Impulse() :
Module{{
    // All modules must initialize these properties
    .name =  "Impulse",
    .inletNumber = 2,
    .outletNumber = 3,
    .width = 400,
    .height = 200
}},
decayDial(5, 100, " ms", 2, this),
shapeDial(0, 1, " %", 0, this)
{
    
    shapeDial.textFromValueFunction = [] (float f) -> String { return String(int(f * 100)); };
    shapeDial.valueFromTextFunction = [] (String s) -> float { return float(s.toUTF8().getDoubleValue()) * 0.01; };

    addAndMakeVisible(decayDial);
    addAndMakeVisible(shapeDial);
    
}

module_Impulse::~module_Impulse()
{
    
}

//==============================================================================
const float PI = MathConstants<float>::pi;
static void drawWaveform(juce::Graphics &g, const Rectangle<float> &viewPort, float shape, float decay, Colour colour) {
    
    g.setColour(colour);
    
    const int pixelsPerPoint = 2;
    const float strokeWidth = 2;
    
    const float startX          =  viewPort.getX();
    const float centreY         =  viewPort.getCentreY();
    const float yRange          =  (viewPort.getHeight() * 0.5) - 3;
    const float width           =  viewPort.getWidth();
    const float endX            =  startX+width;
    const float shapeValue      =  1.006 - shape;
    const int   aaValue         =  8; // x8 AA
    const float phaseIncrement  =  ((7/width)*pixelsPerPoint)/aaValue; // go up to x=7 (arbitrary value)
    
    float phase = 0;
    
    // Waveform path
    Path wavePath;
    wavePath.startNewSubPath (startX, centreY);

    // Add lines to path
    for (int x=startX; x<endX; x += pixelsPerPoint){
        float y = 0;
        for(int i=0; i<aaValue; i++)
        {
            const float lengthPhase = phase*decay;
            // This function is not safe for audio routines! it has a singularity at x=PI :)
            const float newY = (lengthPhase==PI) ? 0.f : sin( sin(lengthPhase) / (shapeValue * (lengthPhase-PI)) );
            y += abs(newY);
            phase += phaseIncrement;
        }
        y /= aaValue;
        wavePath.lineTo (x, y * yRange + centreY);
    }
    
    wavePath.lineTo (endX, centreY);
    
    wavePath = wavePath.createPathWithRoundedCorners(60);

    g.setGradientFill(ColourGradient().vertical(colour, centreY+yRange, colour.darker(), centreY));
    g.fillPath(wavePath);
    g.strokePath (wavePath, PathStrokeType (2.0f));
    
    g.setGradientFill(ColourGradient().vertical(colour, centreY-yRange, colour, centreY));
    wavePath.applyTransform(AffineTransform().verticalFlip(centreY+yRange+strokeWidth*2));
    g.fillPath(wavePath);
    g.strokePath (wavePath, PathStrokeType (strokeWidth));
}

//==============================================================================
void module_Impulse::paint (Graphics& g)
{
    drawWaveform(g, waveViewPort, shapeDial.getValue(), decayDial.getValue(), findColour(Slider::thumbColourId));
}

void module_Impulse::resized()
{

    // The local bounds
    Rectangle<int> moduleBounds = getLocalBounds();
    
    // This line is mandatory for all modules with inlets/outlets
    moduleBounds = placeInletsOutlets( moduleBounds );
    
    // Place the Dials
    Rectangle<int> dialBounds = moduleBounds.removeFromLeft(getWidth()*0.3);
    decayDial.setBounds( dialBounds.removeFromTop(getHeight()*0.5));
    shapeDial.setBounds( dialBounds.removeFromBottom(getHeight()*0.5));
    
    waveViewPort = moduleBounds.reduced(10,0).toFloat();
}

void module_Impulse::decayDialChanged (float value)
{
}

void module_Impulse::shapeDialChanged (float value)
{
}

void module_Impulse::sliderValueChanged (Slider* slider)
{
    if (slider == &decayDial){
        decayDialChanged(slider->getValue());
        repaint();
    } else if (slider == &shapeDial){
        shapeDialChanged(slider->getValue()*0.01);
        repaint();
    }
}
