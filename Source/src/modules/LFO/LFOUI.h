/*
  ==============================================================================

    LFOUI.h
    Created: 20 Aug 2025 15:46:03am
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

///@cond
#include <JuceHeader.h>
///@endcond
#include "../../ModuleProcessor.h"
#include "../../ui/PhiDial.h"


struct LFOUI : ModuleUI
{
    LFOUI(ModuleProcessor& processor) :
    ModuleUI({
        // All modules must initialize these properties
        .name =  "LFO",
        .inlets = {},
        .outlets = {"Out"},
        .width = 260,
        .height = 130,
        .minimumHeight = 100,
        .processor = processor
    }),
    rateDial(*processor.params.getParameter("rate")),
    waveDial(*processor.params.getParameter("wave")),
    shapeDial(*processor.params.getParameter("shape"))
    {
        addAndMakeVisible(waveform);
        addAndMakeVisible(rateDial);
        addAndMakeVisible(waveDial);
        addAndMakeVisible(shapeDial);
        
        waveDial.onValueChange = [&](){updateWaveform();};
        shapeDial.onValueChange = [&](){updateWaveform();};
    }
    
    ~LFOUI() {};

    void paint (Graphics& g) override {};
    
    void onResize(Rectangle<int> moduleBounds) override
    {
        int dialWidth = moduleBounds.getWidth() / 4;
        
        waveform.setBounds( moduleBounds.removeFromLeft(dialWidth) );
        rateDial.setBounds( moduleBounds.removeFromLeft(dialWidth) );
        waveDial.setBounds( moduleBounds.removeFromLeft(dialWidth) );
        shapeDial.setBounds( moduleBounds );
        
        updateWaveform();
    }
    
    void lookAndFeelChanged() override
    {
        waveform.setColour(findColour(Slider::thumbColourId), findColour(Slider::rotarySliderOutlineColourId));
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
        
        const void updateForm(LFO::Wave wave, float shape)
        {
            const int pixelsPerPoint = 2;
            
            const float width = getWidth();
            
            path.clear();
            path.startNewSubPath (strokeWidth, centreY);
            
            auto pos = [&] (float x) {
                return (float)x / (float)width;
            };

            // Add lines to path
            for ( int x = strokeWidth; x < width; x += pixelsPerPoint )
            {
                float y = 0.0f;
                
                if (wave == LFO::Wave::Sine)
                    y = LFO::get_sine(pos(x), shape);
                else if (wave == LFO::Wave::Triangle)
                    y = LFO::get_triangle(pos(x), shape);
                else if (wave == LFO::Wave::Square)
                    y = LFO::get_square(pos(x), shape);
                else if (wave == LFO::Wave::Random) {
                    float pos4 = pos(x) * 4.0f; // Show four cycles in Random
                    y = LFO::get_random(randomValues[(int)pos4], randomValues[(int)pos4+1], pos4, shape);
                }
                
                path.lineTo ( x, -y * yRange + centreY );
            }
            
            path.lineTo ( width, centreY );
            path = path.createPathWithRoundedCorners(60);
            
            repaint();
        }
        
        void resized() override
        {
            centreY = getBounds().toFloat().getCentreY();
            yRange = (float)getHeight() * 0.5f;
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
        
        const float randomValues[5] {0.0f, 0.5f, -0.75f, -0.25f, 0.0f};
    };
    
    Waveform waveform;
    
    PhiDial rateDial, waveDial, shapeDial;
    
    void updateWaveform() {
        waveform.updateForm((LFO::Wave)floor(waveDial.getValue()), shapeDial.getValue() * 0.01f);
    }
};
