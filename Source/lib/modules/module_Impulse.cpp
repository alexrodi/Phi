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
#include "module_Impulse.h"

//==============================================================================
module_Impulse::module_Impulse() :
Module{{
    // All modules must initialize these properties
    .name =  "Impulse",
    .inlets = {"Freq", "Shape"},
    .outlets = {"R", "L", "Ramp"},
    .width = 400,
    .height = 200,
    .minimumHeight = 100
}},
frequencyDial(20, 20000, this, 0.2, " Hz"),
shapeDial(0, 1, this, 1, " %", 0)
{
    shapeDial.textFromValueFunction = [] (float f) -> String { return String(int(f * 100)); };
    shapeDial.valueFromTextFunction = [] (String s) -> float { return float(s.toUTF8().getDoubleValue()) * 0.01; };

    addAndMakeVisible(frequencyDial);
    addAndMakeVisible(shapeDial);
}

module_Impulse::~module_Impulse()
{
    
}


//==============================================================================

void module_Impulse::prepareToPlay (double sampleRate, int maximumExpectedSamplesPerBlock)
{
}

void module_Impulse::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    for (int channel = 0; channel < outletNumber; channel++)
    {
        float* writeBuffer = buffer.getWritePointer(channel);
        for (int n = 0; n < buffer.getNumSamples(); n++)
        {
            *writeBuffer++ = fmod(n*0.01, 1); // a sawtooth-like sound only for testing
        }
    }
    
}

void module_Impulse::releaseResources()
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
    colourGradient = ColourGradient().vertical(colour, centreY+yRange, colour.darker(), centreY);
}

const void module_Impulse::Waveform::draw(Graphics& g)
{
    g.setGradientFill(colourGradient);
    g.fillPath(topPath);
    g.strokePath (topPath, PathStrokeType (strokeWidth));
    
    g.setColour(colour);
    g.fillPath(bottomPath);
    g.strokePath (bottomPath, PathStrokeType (strokeWidth));
}


const void module_Impulse::Waveform::updateForm(const float shape,const  float decay)
{
    
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
            const float newY = (lengthPhase==float_Pi) ? 0.f : sin( sin(lengthPhase) / (shapeValue * (lengthPhase-float_Pi)) );
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

void module_Impulse::wasResized(Rectangle<int> moduleBounds)
{
    // Place the Dials
    Rectangle<int> dialBounds = moduleBounds.removeFromLeft(getWidth()*0.25);
    frequencyDial.setBounds( dialBounds.removeFromTop(getHeight()*0.5));
    shapeDial.setBounds( dialBounds.removeFromBottom(getHeight()*0.5));
    
    // Place the waveform and update
    waveForm.setViewPort(moduleBounds.reduced(10,0).toFloat());
    waveForm.updateForm(shapeDial.getValue(), frequencyDial.getValue());
}

void module_Impulse::lookAndFeelChanged()
{
    waveForm.setColour(findColour(Slider::thumbColourId));
}

void module_Impulse::frequencyDialChanged (float value)
{
}

void module_Impulse::shapeDialChanged (float value)
{
}

void module_Impulse::sliderValueChanged (Slider* slider)
{
    bool isDecay = slider == &frequencyDial;
    bool isShape = slider == &shapeDial;
    if (isDecay || isShape)
    {
        if (isDecay){
            frequencyDialChanged(slider->getValue());
        } else if (isShape){
            shapeDialChanged(slider->getValue()*0.01);
        }
        waveForm.updateForm(shapeDial.getValue(), frequencyDial.getValue());
        repaint();
    }
    
}
