/*
  ==============================================================================

    Impulse.h
    Created: 9 Feb 2020 11:02:38pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

///@cond
#include <JuceHeader.h>
///@endcond
#include "../../ModuleProcessor.h"
#include "../../ui/PhiDial.h"
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
        .width = 370,
        .height = 220,
        .minimumHeight = 100,
        .processor = processor
    }},
    frequencyDial(*processor.params.getParameter("freq")),
    shapeDial(*processor.params.getParameter("shape"))
    {
        waveForm.setInterceptsMouseClicks(false, false);
        waveForm.setPaintingIsUnclipped(true);
        waveForm.setBufferedToImage(true);
        
        addAndMakeVisible(waveForm);
        addAndMakeVisible(frequencyDial);
        addAndMakeVisible(shapeDial);
        
        shapeDial.onValueChange = [&] () {waveForm.updateForm(shapeDial.getValue());};
    }
    
    ~ImpulseUI() {};

    void paint (Graphics&) override {}
    
    void onResize(Rectangle<int> moduleBounds) override
    {
        // Place the Dials
        Rectangle<int> dialBounds = moduleBounds.removeFromLeft(getWidth()*0.25);
        frequencyDial.setBounds( dialBounds.removeFromTop(getHeight()*0.48));
        shapeDial.setBounds( dialBounds.removeFromBottom(getHeight()*0.48));
        
        // Place the waveform and update
        waveForm.setBounds(moduleBounds.reduced(10,0));
        waveForm.updateForm(shapeDial.getValue());
    }
    
    void mouseDown(const MouseEvent& e) override
    {
        *props.processor.params.getRawParameterValue("trigger") = 1.0f;
    }
    
    void lookAndFeelChanged() override
    {
        waveForm.setColour(findColour(Slider::thumbColourId), findColour(Slider::rotarySliderOutlineColourId));
    }

private:
    struct Waveform : Component
    {
        Waveform()
        {
            setBufferedToImage(true);
            setPaintingIsUnclipped(true);
        }
        
        const void setColour(const Colour& newStrokeColour, const Colour& newFillColour)
        {
            strokeColour = newStrokeColour;
            fillColour = newFillColour;
        }
        
        const void updateForm(const float shape)
        {
            const float shapeValue = powf(shape, 0.1f);
            
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
                    y += fabsf( processImpulse( phase, shapeValue ) );
                    phase += phaseIncrement;
                }
                y *= aaRatio;
                path.lineTo ( x, y * yRange + centreY );
            }
            
            path.lineTo ( width, centreY );
            path = path.createPathWithRoundedCorners(60);
            
            auto bottomPath = path;
            bottomPath.applyTransform( AffineTransform().verticalFlip( (float)getHeight() ) );
            path.addPath(bottomPath);
            repaint();
        }
        
        void resized() override
        {
            centreY = getBounds().toFloat().getCentreY();
            yRange = (float)getHeight() * 0.57f;
        }
        
        void paint(Graphics& g) override
        {
            g.setColour(fillColour);
            g.fillPath(path);
            
            g.setColour(strokeColour);
            g.strokePath (path, PathStrokeType (strokeWidth));
        }
    private:
        const float strokeWidth = 1;
        float yRange;
        float centreY;
        Colour strokeColour;
        Colour fillColour;
        Path path;
    };

    Waveform waveForm;
    
    PhiDial frequencyDial, shapeDial;
};
