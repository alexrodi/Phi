/*
  ==============================================================================

    phi_Oscillator.cpp
    Created: 9 Feb 2020 11:02:38pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#include <JuceHeader.h>
#include "phi_Oscillator.h"

//==============================================================================
phi_Oscillator::phi_Oscillator() :
frequencySlider(50, 5000.0, " Hz", 2, this),
shapeSlider(0, 100, " %", 0, this)
{

    setPaintingIsUnclipped(true);

    addAndMakeVisible(frequencySlider);
    addAndMakeVisible(shapeSlider);
}

phi_Oscillator::~phi_Oscillator()
{
}

//==============================================================================
static void drawSine(juce::Graphics &g, const Rectangle<float> &viewPort, float shape) {
    
    const float yRange = (viewPort.getHeight() * 0.5) - 3;
    
    if(yRange > 5){
        const int pixelsPerPoint = 2;
        
        const float x = viewPort.getX();
        const float centreY = viewPort.getCentreY();
        const float width = viewPort.getWidth();
        const float maxX = x+width;
        const float phaseIncrement = (6.2831853072/width)*pixelsPerPoint;
        
        Path wavePath;
        
        wavePath.startNewSubPath (x, centreY);
        
        float phase = 0;
        for (int pixel=x; pixel<maxX; pixel += pixelsPerPoint){
            wavePath.lineTo (pixel, sin(sin(phase) * (1+shape*0.1))*yRange + centreY);
            phase += phaseIncrement;
        }
        
        g.strokePath (wavePath, PathStrokeType (2.0f));
    }
}

//==============================================================================
void phi_Oscillator::paint (Graphics& g)
{
    g.setColour(findColour(Slider::thumbColourId));
    
    drawSine(g, waveViewPort, shapeSlider.getValue());
}

void phi_Oscillator::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

    Rectangle<int> moduleBounds = getLocalBounds();
    Rectangle<int> sliderBounds = moduleBounds.removeFromLeft(getWidth()*0.3);
    frequencySlider.setBounds( sliderBounds.removeFromTop(getHeight()*0.5) );
    shapeSlider.setBounds( sliderBounds );
    
    waveViewPort = moduleBounds.reduced(10,0).toFloat();
}

void phi_Oscillator::frequencySliderChanged (float value)
{
}

void phi_Oscillator::shapeSliderChanged (float value)
{
    repaint();
}

void phi_Oscillator::sliderValueChanged (Slider* slider)
{
    if (slider == &frequencySlider){
        frequencySliderChanged(slider->getValue());
        repaint();
    } else if (slider == &shapeSlider){
        shapeSliderChanged(slider->getValue()*0.01);
        repaint();
    }
}
