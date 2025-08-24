/*
  ==============================================================================

    LFOUI.h
    Created: 20 Aug 2025 15:46:03am
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

#include "../../ui/ModuleUI.h"
#include "../../ui/component/PhiDial.h"


struct LFOUI : ModuleUI
{
    LFOUI(ModuleProcessor& processor) :
    ModuleUI({
        // All modules must initialize these properties
        .name =  "LFO",
        .inlets = {"Rate", "Shape"},
        .outlets = {"Out"},
        .defaultSize = {270, 180},
        .minimumSize = {245, 145},
        .processor = processor
    }),
    rateDial(*processor.params.getParameter("rate")),
    waveDial(*processor.params.getParameter("wave")),
    shapeDial(*processor.params.getParameter("shape"))
    {
        addAndMakeVisible(waveform);
        addAndMakeVisible(waveDial);
        addAndMakeVisible(rateDial);
        addAndMakeVisible(shapeDial);
        
        waveDial.onValueChange = [&](){updateWaveform();};
        rateDial.onValueChange = [&](){updateWaveform();};
        shapeDial.onValueChange = [&](){updateWaveform();};
    }
    
    ~LFOUI() {};

    void paint (juce::Graphics& g) override {};
    
    void resized() override
    {
        auto bounds = getLocalBounds();
        
        waveform.setBounds(bounds.removeFromBottom(bounds.getHeight() / 3));
        
        bounds.removeFromBottom(5);
        
        int dialWidth = bounds.getWidth() / 3;
        
        waveDial.setBounds(bounds.removeFromLeft(dialWidth));
        rateDial.setBounds(bounds.removeFromLeft(dialWidth));
        shapeDial.setBounds(bounds);
        
        updateWaveform();
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
            
            path = path.createPathWithRoundedCorners(pixelsPerPoint);
            
            repaint();
        }
        
        void resized() override {}
        
        void paint(juce::Graphics& g) override
        {
            g.setColour(fillColour);
            g.fillRect(getLocalBounds());
            
            g.setColour(strokeColour);
            g.strokePath (path, juce::PathStrokeType (strokeWidth));
        }
    private:
        static constexpr int RATE_MULT = 4;
        const int pixelsPerPoint = 2;
        const float strokeWidth = 1;
        juce::Colour strokeColour;
        juce::Colour fillColour;
        juce::Path path;
        
        const float randomValues[RATE_MULT+2] = {0.0f, 0.5f, -0.75f, -0.25f, 0.0f, 0.25f};
    };
    
    Waveform waveform;
    
    PhiDial rateDial, waveDial, shapeDial;
    
    void updateWaveform() {
        waveform.updateForm(
            rateDial.getValue(),
            (LFO::Wave)floor(waveDial.getValue()),
            shapeDial.getValue() * 0.01f
        );
    }
};
