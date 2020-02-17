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
decaySlider(5, 100, " ms", 2, this),
shapeSlider(0, 1, " %", 0, this)
{
    
    shapeSlider.textFromValueFunction = [] (float f) -> String { return String(f*100); };

    setPaintingIsUnclipped(true);

    addAndMakeVisible(decaySlider);
    addAndMakeVisible(shapeSlider);
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
        
        const float x = viewPort.getX();
        const float centreY = viewPort.getCentreY();
        const float width = viewPort.getWidth();
        const float maxX = x+width;
        const float phaseIncrement = (6.2831853072/width)*pixelsPerPoint;
        const float shapeValue = 1.006 - shape;
        
        
        Path wavePath;
        
        wavePath.startNewSubPath (x, centreY);
        
        float phase = 0;
        for (int pixel=x; pixel<maxX; pixel += pixelsPerPoint){
            const float lengthPhase = phase*decay;
            wavePath.lineTo (pixel, sin( sin(lengthPhase) / (shapeValue * (lengthPhase-PI)) ) * yRange + centreY);
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
