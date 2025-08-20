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
        .height = 180,
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
        
        rateDial.onValueChange = [&](){updateWaveform();};
        waveDial.onValueChange = [&](){updateWaveform();};
        shapeDial.onValueChange = [&](){updateWaveform();};
    }
    
    ~LFOUI() {};

    void paint (Graphics& g) override {};
    
    void onResize(Rectangle<int> moduleBounds) override
    {
        waveform.setBounds( moduleBounds.removeFromBottom(moduleBounds.getHeight() / 3) );
        
        moduleBounds.removeFromBottom(separatorHeight);
        
        int dialWidth = moduleBounds.getWidth() / 3;
        
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
        
        const void updateForm(float rate, LFO::Wave wave, float shape)
        {
            auto bounds = getLocalBounds().reduced(2).toFloat();
            float centerY = bounds.getCentreY();
            float yRange = (float)bounds.getHeight() * 0.48f;
            
            int startX = bounds.getX() + strokeWidth;
            int endX = bounds.getRight() - strokeWidth;
            
            float rateFactor = rate * (float)RATE_MULT + 1.0f;
            
            auto pos = [&] (float x) {
                return ((float)x * rateFactor) / (float)getWidth();
            };
            
            path.clear();
            
            // Add lines to path
            for ( int x = startX; x < endX; x += pixelsPerPoint )
            {
                float y = centerY;
                
                if (wave == LFO::Wave::Sine)
                    y -= LFO::get_sine(pos(x), shape) * yRange;
                else if (wave == LFO::Wave::Triangle)
                    y -= LFO::get_triangle(pos(x), shape) * yRange;
                else if (wave == LFO::Wave::Square)
                    y -= LFO::get_square(pos(x), shape) * yRange;
                else if (wave == LFO::Wave::Random) {
                    float val = pos(x);
                    y -= LFO::get_random(randomValues[(int)val], randomValues[(int)val+1], val, shape) * yRange;
                }
                
                
                
                if (path.isEmpty())
                    path.startNewSubPath (x, y);
                
                path.lineTo (x, y);
            }
            
            path = path.createPathWithRoundedCorners(60);
            
            repaint();
        }
        
        void resized() override {}
        
        void paint(Graphics& g) override
        {
            g.setColour(fillColour);
            g.fillRect(getLocalBounds());
            
            g.setColour(strokeColour);
            g.strokePath (path, PathStrokeType (strokeWidth));
        }
    private:
        static constexpr int RATE_MULT = 4;
        const int pixelsPerPoint = 2;
        const float strokeWidth = 1;
        Colour strokeColour;
        Colour fillColour;
        Path path;
        
        const float randomValues[RATE_MULT+2] = {0.0f, 0.5f, -0.75f, -0.25f, 0.0f, 0.25f};
    };
    
    Waveform waveform;
    
    PhiDial rateDial, waveDial, shapeDial;
    
    const int separatorHeight = 5;
    
    void updateWaveform() {
        waveform.updateForm(
            rateDial.getValue(),
            (LFO::Wave)floor(waveDial.getValue()),
            shapeDial.getValue() * 0.01f
        );
    }
};
