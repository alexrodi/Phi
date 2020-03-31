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

// Waveform
//==============================================================================

const void module_Impulse::Waveform::setViewPort(const Rectangle<float> viewportToUse)
{
    viewPort = viewportToUse;
    
    centreY = viewPort.getCentreY();
    yRange = viewPort.getHeight() * 0.5 - 3;
    updateColour();
}

const void module_Impulse::Waveform::setColour(const Colour& colourToUse)
{
    colour = colourToUse;
    updateColour();
}

const void module_Impulse::Waveform::updateColour()
{
    topColourGradient = ColourGradient().vertical(colour, centreY+yRange, colour.darker(), centreY);
    bottomColourGradient = ColourGradient().vertical(colour, centreY-yRange, colour, centreY);
}

const void module_Impulse::Waveform::draw(Graphics& g)
{
    g.setGradientFill(topColourGradient);
    g.fillPath(topPath);
    g.strokePath (topPath, PathStrokeType (strokeWidth));
    
    g.setGradientFill(bottomColourGradient);
    g.fillPath(bottomPath);
    g.strokePath (bottomPath, PathStrokeType (strokeWidth));
}


const void module_Impulse::Waveform::updateForm(const float shape,const  float decay)
{
    const float PI = MathConstants<float>::pi;
    
    const int pixelsPerPoint = 2;
    
    const float startX          =  viewPort.getX();
    const float width           =  viewPort.getWidth();
    const float endX            =  startX+width;
    const float shapeValue      =  1.006 - shape;
    const int   aaValue         =  8; // x8 AA
    const float phaseIncrement  =  ((7/width)*pixelsPerPoint)/aaValue; // go up to x=7 (arbitrary value)
    
    float phase = 0;
    
    topPath.clear();
    topPath.startNewSubPath (startX, centreY);

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
        topPath.lineTo (x, y * yRange + centreY);
    }
    
    topPath.lineTo (endX, centreY);
    
    topPath = topPath.createPathWithRoundedCorners(60);
    bottomPath = topPath;
    bottomPath.applyTransform(AffineTransform().verticalFlip(centreY+yRange+strokeWidth*2));
}
//==============================================================================

void module_Impulse::paint (Graphics& g)
{
    waveForm.draw(g);
}

void module_Impulse::resized()
{

    // The local bounds
    Rectangle<int> moduleBounds = getLocalBounds();
    
    // This line is mandatory for all modules with inlets/outlets
    moduleBounds = placeInletsOutlets( moduleBounds );
    
    // Place the Dials
    Rectangle<int> dialBounds = moduleBounds.removeFromLeft(getWidth()*0.25);
    decayDial.setBounds( dialBounds.removeFromTop(getHeight()*0.5));
    shapeDial.setBounds( dialBounds.removeFromBottom(getHeight()*0.5));
    
    waveForm.setViewPort(moduleBounds.reduced(10,0).toFloat());
    waveForm.updateForm(shapeDial.getValue(), decayDial.getValue());
}

void module_Impulse::lookAndFeelChanged()
{
    waveForm.setColour(findColour(Slider::thumbColourId));
}

void module_Impulse::decayDialChanged (float value)
{
}

void module_Impulse::shapeDialChanged (float value)
{
}

void module_Impulse::sliderValueChanged (Slider* slider)
{
    bool isDecay = slider == &decayDial;
    bool isShape = slider == &shapeDial;
    if (isDecay || isShape)
    {
        if (isDecay){
            decayDialChanged(slider->getValue());
        } else if (isShape){
            shapeDialChanged(slider->getValue()*0.01);
        }
        waveForm.updateForm(shapeDial.getValue(), decayDial.getValue());
        repaint();
    }
    
}
