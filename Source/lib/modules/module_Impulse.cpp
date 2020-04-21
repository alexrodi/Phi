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
    .inlets = {"Trigger", "Freq", "Shape"},
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
    
    waveForm.setInterceptsMouseClicks(false, false);

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
    const bool readInputs = ! buffer.hasBeenCleared();
    
    const float* readBufferTrigger = buffer.getReadPointer(0);
    //const float* readBufferFreq = buffer.getReadPointer(1);
    //const float* readBufferShape = buffer.getReadPointer(2);
    
    const float shape = powf(shapeDial.getValue(), 0.1f);
    
    const float phaseIncrement = (1.0f/float(sampleRate/frequencyDial.getValue())) * MathConstants<float>::twoPi;
    
    float* writeBufferOut = buffer.getWritePointer(0);
    float* writeBufferRamp = buffer.getWritePointer(1);
    
    if (readInputs)
    {
        for (int n = 0; n < buffer.getNumSamples(); n++)
        {
            const float trigger = *readBufferTrigger++;
            const float triggerDelta = currentTrigger - trigger;
            currentTrigger = trigger;
            
            if (triggerDelta > 0.5f)
            {
                triggerImpulse();
            }
            
            *writeBufferOut++ = processImpulse(currentPhase, shape);
            *writeBufferRamp++ = currentPhase * invTwoPi;

            currentPhase += phaseIncrement;
        }
    }
    else
    {
        for (int n = 0; n < buffer.getNumSamples(); n++)
        {
            *writeBufferOut++ = processImpulse(currentPhase, shape);
            *writeBufferRamp++ = currentPhase * invTwoPi;

            currentPhase += phaseIncrement;
        }
    }
    
}

void module_Impulse::releaseResources()
{
}

void module_Impulse::triggerImpulse()
{
    currentPhase = 0;
}

float module_Impulse::processImpulse(float phase, float shape)
{
    const float fundamentalAttenuator = (-0.5*tanhf(phase*(-fmax(shape,0.88)+1.01)-1)+0.5);

    return (phase==float_Pi)
           ? 0.f
           : sin((sin(phase))/((-shape + 1.006)*(phase-float_Pi)))*fundamentalAttenuator;
}


// Waveform
//==============================================================================

void module_Impulse::mouseDown(const MouseEvent& e)
{
    triggerImpulse();
}

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
    const float shapeValue = powf(shape, 0.1f);
    
    const int pixelsPerPoint = 2;
    
    const float width           =  getWidth();
    const int   aaValue         =  8; // x8 AA
    // scale the values so that the waveform (more or less) fills the width
    const float phaseIncrement  =  (((powf(shapeValue,50)*200 + 30)/width)*pixelsPerPoint)/aaValue;
    
    float phase = 0;
    
    topPath.clear();
    topPath.startNewSubPath (0, centreY);

    // Add lines to path
    for (int x=0; x<width; x += pixelsPerPoint){
        float y = 0;
        for(int i=0; i<aaValue; i++)
        {
            y += fabs(processImpulse(phase, shapeValue));
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
