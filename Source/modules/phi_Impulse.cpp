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
    const int numInlets = 2;
    const int numOutlets = 3;
    
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
static void drawWaveform(juce::Graphics &g, const Rectangle<float> &viewPort, float shape, float decay) {
    
    const float yRange = (viewPort.getHeight() * 0.5) - 3;
    
    if(yRange > 5){
        const int pixelsPerPoint = 2;
        
        const float startX = viewPort.getX();
        const float centreY = viewPort.getCentreY();
        const float width = viewPort.getWidth();
        const float endX = startX+width;
        const float phaseIncrement = (7/width)*pixelsPerPoint; // go up to x=7 as an arbitrary value
        const float shapeValue = 1.006 - shape;
        
        
        Path wavePath;
        
        wavePath.startNewSubPath (startX, centreY);
        
        float phase = 0;
        for (int x=startX; x<endX; x += pixelsPerPoint){
            const float lengthPhase = phase*decay;
            // This function is not safe for audio routines! it has a singularity at x=PI :)
            const float y = (lengthPhase==PI) ? 0.f : sin( sin(lengthPhase) / (shapeValue * (lengthPhase-PI)) );
            wavePath.lineTo (x, y * yRange + centreY);
            phase += phaseIncrement;
        }
        g.strokePath (wavePath, PathStrokeType (2.0f));
    }
}

//==============================================================================
void phi_Impulse::paint (Graphics& g)
{
    g.setColour(findColour(Slider::thumbColourId));
    
    drawWaveform(g, waveViewPort, shapeDial.getValue(), decayDial.getValue());
}

void phi_Impulse::resized()
{

    // The local bounds
    Rectangle<int> moduleBounds = getLocalBounds();
    
    // Place the inlets (equidistant)
    Rectangle<int> inletBounds = moduleBounds.removeFromLeft(30);
    for (phi_Inlet* inlet : inlets)
        inlet->setBounds( inletBounds.removeFromTop(moduleBounds.getHeight()/inlets.size()) );
    
    // Place the outlets (equidistant)
    Rectangle<int> outletBounds = moduleBounds.removeFromRight(30);
    for (phi_Outlet* outlet : outlets)
        outlet->setBounds( outletBounds.removeFromTop(moduleBounds.getHeight()/outlets.size()) );
    
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
    repaint();
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
