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
decaySlider(5, 100, " ms", 2, this),
shapeSlider(0, 1, " %", 0, this)
{
    const int numInlets = 2;
    const int numOutlets = 3;
    
    shapeSlider.textFromValueFunction = [] (float f) -> String { return String(f*100); };

    setPaintingIsUnclipped(true);

    addAndMakeVisible(decaySlider);
    addAndMakeVisible(shapeSlider);
    
    for (int i=0; i<numInlets; i++)
        addAndMakeVisible( inlets.add( new phi_Inlet() ) );
    
    for (int i=0; i<numOutlets; i++)
        addAndMakeVisible( outlets.add( new phi_Outlet() ) );

}

phi_Impulse::~phi_Impulse()
{
    
}

//==============================================================================
const float PI = MathConstants<float>::pi;
static void drawSine(juce::Graphics &g, const Rectangle<float> &viewPort, float shape, float decay) {
    
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
    
    drawSine(g, waveViewPort, shapeSlider.getValue(), decaySlider.getValue());
}

void phi_Impulse::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

    Rectangle<int> moduleBounds = getLocalBounds();
    
    Rectangle<int> inletBounds = moduleBounds.removeFromLeft(30);
    for (phi_Inlet* inlet : inlets)
        inlet->setBounds( inletBounds.removeFromTop(moduleBounds.getHeight()/inlets.size()) );
    
    Rectangle<int> outletBounds = moduleBounds.removeFromRight(30);
    for (phi_Outlet* outlet : outlets)
        outlet->setBounds( outletBounds.removeFromTop(moduleBounds.getHeight()/outlets.size()) );
    
    Rectangle<int> sliderBounds = moduleBounds.removeFromLeft(getWidth()*0.3);
    decaySlider.setBounds( sliderBounds.removeFromTop(getHeight()*0.5) );
    shapeSlider.setBounds( sliderBounds );
    
    waveViewPort = moduleBounds.reduced(10,0).toFloat();
}

void phi_Impulse::decaySliderChanged (float value)
{
}

void phi_Impulse::shapeSliderChanged (float value)
{
    repaint();
}

void phi_Impulse::sliderValueChanged (Slider* slider)
{
    if (slider == &decaySlider){
        decaySliderChanged(slider->getValue());
        repaint();
    } else if (slider == &shapeSlider){
        shapeSliderChanged(slider->getValue()*0.01);
        repaint();
    }
}
