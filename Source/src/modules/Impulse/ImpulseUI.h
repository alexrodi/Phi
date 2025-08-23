/*
  ==============================================================================

    Impulse.h
    Created: 9 Feb 2020 11:02:38pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

#include "../../ui/ModuleUI.h"
#include "../../ui/component/PhiDial.h"
//==============================================================================
/*
*/
struct ImpulseUI : ModuleUI
{
    ImpulseUI(ModuleProcessor& processor) :
    ModuleUI{{
        // All modules must initialize these properties
        .name =  "Impulse",
        .inlets = {"Trigger", "Freq", "Shape"},
        .outlets = {"Out", "Ramp"},
        .width = 220,
        .height = 220,
        .minimumHeight = 100,
        .processor = processor
    }},
    frequencyDial(*processor.params.getParameter("freq")),
    shapeDial(*processor.params.getParameter("shape"))
    {
        waveform.setInterceptsMouseClicks(false, false);
        waveform.setPaintingIsUnclipped(true);
        waveform.setBufferedToImage(true);
        
        addAndMakeVisible(waveform);
        addAndMakeVisible(frequencyDial);
        addAndMakeVisible(shapeDial);
        
        shapeDial.onValueChange = [&] () {updateWaveform();};
    }
    
    ~ImpulseUI() {};

    void paint (juce::Graphics&) override {}
    
    void resized() override
    {
        auto bounds = getLocalBounds();
        
        // Place the waveform and update
        waveform.setBounds(bounds.removeFromBottom(bounds.getHeight() / 2));
        updateWaveform();
        
        bounds.removeFromBottom(5);
        
        // Place the Dials
        frequencyDial.setBounds(bounds.removeFromLeft(bounds.getWidth() / 2));
        shapeDial.setBounds(bounds);
    }
    
    void mouseDown(const juce::MouseEvent& e) override
    {
        *props.processor.params.getRawParameterValue("trigger") = 1.0f;
    }
    
    void lookAndFeelChanged() override
    {
        waveform.setColour(findColour(juce::Slider::thumbColourId), findColour(juce::Slider::rotarySliderOutlineColourId));
    }

private:
    struct Waveform : Component
    {
        Waveform()
        {
            setBufferedToImage(true);
            setPaintingIsUnclipped(true);
        }
        
        const void setColour(const juce::Colour& newStrokeColour, const juce::Colour& newFillColour)
        {
            strokeColour = newStrokeColour;
            fillColour = newFillColour;
        }
        
        const void updateForm(const float shape)
        {
            const float shapeValue = powf(shape, 0.2f);
            
            const int pixelsPerPoint = 2;
            
            const float width           =  getWidth();
            const int   aaValue         =  8; // x8 AA
            const float aaRatio         =  1.0f / aaValue;
            // scale the values so that the waveform (more or less) fills the width
            const float phaseIncrement  =  ( ( ( powf( shapeValue, 50.0f ) * 200.0f + 30.0f) / width) * (float)pixelsPerPoint) * aaRatio;
            
            float phase = 0.0f;
            
            path.clear();
            path.startNewSubPath (strokeWidth, centreY);

            // Add lines to path
            for ( int x = strokeWidth; x < width; x += pixelsPerPoint )
            {
                float y = 0.0f;
                for( int i = 0; i < aaValue; i++ )
                {
                    y += abs(processImpulse( phase, shapeValue ));
                    phase += phaseIncrement;
                }
                y *= aaRatio;
                path.lineTo ( x, y * yRange + centreY );
            }
            
            path.lineTo ( width, centreY );
            path = path.createPathWithRoundedCorners(pixelsPerPoint);
            
            auto bottomPath = path;
            bottomPath.applyTransform( juce::AffineTransform().verticalFlip( (float)getHeight() ) );
            path.addPath(bottomPath);
            repaint();
        }
        
        void resized() override
        {
            centreY = getLocalBounds().toFloat().getCentreY();
            yRange = (float)getHeight() * 0.55f;
        }
        
        void paint(juce::Graphics& g) override
        {
            g.setColour(fillColour);
            g.fillRect(getLocalBounds());
            
            g.setColour(strokeColour);
            g.strokePath (path, juce::PathStrokeType (strokeWidth));
        }
    private:
        const float strokeWidth = 1;
        float yRange = 1.0, centreY = 0.0f;
        juce::Colour strokeColour;
        juce::Colour fillColour;
        juce::Path path;
    };

    Waveform waveform;
    
    PhiDial frequencyDial, shapeDial;
    
    void updateWaveform() {
        waveform.updateForm(shapeDial.getValue() * 0.01f);
    }
};
