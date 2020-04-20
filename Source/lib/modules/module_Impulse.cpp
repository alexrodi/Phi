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

const float invTwoPi = 1.0f/MathConstants<float>::twoPi;

//==============================================================================
module_Impulse::module_Impulse() :
Module{{
    // All modules must initialize these properties
    .name =  "Impulse",
    .inlets = {"Freq", "Shape"},
    .outlets = {"Out", "Ramp"},
    .width = 400,
    .height = 200,
    .minimumHeight = 100
}},
frequencyDial(20, 20000, this, 0.2, " Hz"),
shapeDial(0, 1, this, 1, " %", 0)
{
    shapeDial.textFromValueFunction = [] (float f) -> String { return String(int(f * 100)); };
    shapeDial.valueFromTextFunction = [] (String s) -> float { return float(s.toUTF8().getDoubleValue()) * 0.01; };

    addAndMakeVisible(waveForm);
    addAndMakeVisible(frequencyDial);
    addAndMakeVisible(shapeDial);
}

module_Impulse::~module_Impulse()
{
    
}


//==============================================================================

void module_Impulse::prepareToPlay (double newSampleRate, int maximumExpectedSamplesPerBlock)
{
    sampleRate = newSampleRate;
}

void module_Impulse::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    const float shape = shapeDial.getValue();
    
    const float phaseIncrement = (1.0f/float(sampleRate/frequencyDial.getValue())) * MathConstants<float>::twoPi;
    
    float* writeBufferOut = buffer.getWritePointer(0);
    float* writeBufferRamp = buffer.getWritePointer(1);
    
    for (int n = 0; n < buffer.getNumSamples(); n++)
    {
        const float fundamentalAttenuator = (-0.5*tanhf(currentPhase*(-fmax(shape,0.88)+1.01)-1)+0.5);

        const float newSample = (currentPhase==float_Pi)
                                ? 0.f
                                : sin((sin(currentPhase))/((-shape + 1.006)*(currentPhase-float_Pi)))*fundamentalAttenuator;

        *writeBufferOut++ = newSample;
        *writeBufferRamp++ = currentPhase * invTwoPi;

        currentPhase += phaseIncrement;
        if (currentPhase > 300)
        {
            currentPhase = 0;
        }
    }
    
//    float* writeBuffer = buffer.getWritePointer(0);
//
//    for (int n = 0; n < buffer.getNumSamples(); n++)
//    {
//        *writeBuffer++ = sin(n*phaseIncrement);
//    }
    
}

void module_Impulse::releaseResources()
{
}



// Waveform
//==============================================================================

void module_Impulse::Waveform::resized()
{
    centreY = getBounds().getCentreY();
    yRange = getHeight() * 0.5;
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

void module_Impulse::Waveform::paint(Graphics& g)
{
    g.setGradientFill(colourGradient);
    g.fillPath(topPath);
    g.strokePath (topPath, PathStrokeType (strokeWidth));
    
    g.setColour(colour);
    g.fillPath(bottomPath);
    g.strokePath (bottomPath, PathStrokeType (strokeWidth));
}


const void module_Impulse::Waveform::updateForm(const float shape)
{
    
    const int pixelsPerPoint = 2;
    
    const float width           =  getWidth();
    const int   aaValue         =  8; // x8 AA
    const float shapeValue      =  -shape + 1.006;
    const float phaseIncrement  =  (((pow(shape,50)*200 + 30)/width)*pixelsPerPoint)/aaValue; // go up to x=7 (arbitrary value)
    
    float phase = 0;
    
    topPath.clear();
    topPath.startNewSubPath (0, centreY);

    // Add lines to path
    for (int x=0; x<width; x += pixelsPerPoint){
        float y = 0;
        for(int i=0; i<aaValue; i++)
        {
            const float lengthPhase = phase;
            
            const float fundamentalAttenuator = (-0.5*tanh(lengthPhase*(-fmax(shape,0.88)+1.01)-1)+0.5);
            
            const float newY = (lengthPhase==float_Pi)
                                ? 0.f
                                : sin((sin(lengthPhase))/(shapeValue*(lengthPhase-float_Pi)))*fundamentalAttenuator;
            y += abs(newY);
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

void module_Impulse::paint (Graphics& g)
{
}

void module_Impulse::wasResized(Rectangle<int> moduleBounds)
{
    // Place the Dials
    Rectangle<int> dialBounds = moduleBounds.removeFromLeft(getWidth()*0.25);
    frequencyDial.setBounds( dialBounds.removeFromTop(getHeight()*0.5));
    shapeDial.setBounds( dialBounds.removeFromBottom(getHeight()*0.5));
    
    // Place the waveform and update
    waveForm.setBounds(moduleBounds.reduced(10,0));
    waveForm.updateForm(shapeDial.getValue());
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
        waveForm.updateForm(shapeDial.getValue());
        repaint();
    }
    
}
