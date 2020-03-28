/*
  ==============================================================================

    phi_Impulse.cpp
    Created: 9 Feb 2020 11:02:38pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#include <JuceHeader.h>
#include "phi_Impulse.h"

//==============================================================================
phi_Impulse::phi_Impulse() :
moduleName("Impulse"),
decayDial(5, 100, " ms", 2, this),
shapeDial(0, 1, " %", 0, this)
{
    // Declare the number of inlets & outlets here
    const int numInlets = 10;
    const int numOutlets = 13;
    
    shapeDial.textFromValueFunction = [] (float f) -> String { return String(int(f*100)); };
    
    //======================================================
    addAndMakeVisible(decayDial);
    addAndMakeVisible(shapeDial);
    
    for (int i=0; i<numInlets; i++)
        addAndMakeVisible( inlets.add( new phi_Inlet() ) );
    
    for (int i=0; i<numOutlets; i++)
        addAndMakeVisible( outlets.add( new phi_Outlet() ) );

    setPaintingIsUnclipped(true);
}

phi_Impulse::~phi_Impulse()
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
void phi_Impulse::paint (Graphics& g)
{
    drawWaveform(g, waveViewPort, shapeDial.getValue(), decayDial.getValue(), findColour(Slider::thumbColourId));
}

void phi_Impulse::resized()
{

    // The local bounds
    Rectangle<int> moduleBounds = getLocalBounds();
    
    // Place the inlets (equidistant)
    Rectangle<int> inletBounds = moduleBounds.removeFromLeft(30);
    for (phi_Inlet* inlet : inlets)
        inlet->setBounds( inletBounds.removeFromTop((float)moduleBounds.getHeight()/(float)inlets.size()) );
    
    // Place the outlets (equidistant)
    Rectangle<int> outletBounds = moduleBounds.removeFromRight(30);
    for (phi_Outlet* outlet : outlets)
        outlet->setBounds( outletBounds.removeFromTop((float)moduleBounds.getHeight()/(float)outlets.size()) );
    
    // Place the Dials
    Rectangle<int> dialBounds = moduleBounds.removeFromLeft(getWidth()*0.3);
    decayDial.setBounds( dialBounds.removeFromTop(getHeight()*0.5) );
    shapeDial.setBounds( dialBounds );
    
    waveViewPort = moduleBounds.reduced(10,0).toFloat();
}

void phi_Impulse::decayDialChanged (float value)
{
}

void phi_Impulse::shapeDialChanged (float value)
{
}

void phi_Impulse::sliderValueChanged (Slider* slider)
{
    if (slider == &decayDial){
        decayDialChanged(slider->getValue());
        repaint();
    } else if (slider == &shapeDial){
        shapeDialChanged(slider->getValue()*0.01);
        repaint();
    }
}
