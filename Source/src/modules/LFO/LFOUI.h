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
#include "../../ui/component/PhiWaveform.h"

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

private:
    struct LFOWaveform : public PhiWaveform
    {
        LFOWaveform() {
            setAA(8);
        }
        
        void set(float newRate, LFO::Wave newWave, float newShape)
        {
            rate = newRate;
            wave = newWave;
            shape = newShape;
            
            updateWavefom();
        }

    protected:
        float getSample(float phase) const override
        {
            const float rateFactor = rate * (float)RATE_MULT + 1.0f;
            const float lfoPhase = phase * rateFactor;

            if (wave == LFO::Wave::Sine)
                return LFO::get_sine(lfoPhase, shape);
            if (wave == LFO::Wave::Triangle)
                return LFO::get_triangle(lfoPhase, shape);
            if (wave == LFO::Wave::Square)
                return LFO::get_square(lfoPhase, shape);
            if (wave == LFO::Wave::Random) {
                int index = std::min((int)lfoPhase, RATE_MULT);
                return LFO::get_random(randomValues[index], randomValues[index + 1], lfoPhase, shape);
            }
            
            return 0.0f;
        }

    private:
        float rate = 0.0f, shape = 0.0f;
        LFO::Wave wave = LFO::Wave::Sine;

        static constexpr int RATE_MULT = 4;
        const float randomValues[RATE_MULT + 2] = {0.0f, 0.5f, -0.75f, -0.25f, 0.0f, 0.25f};
    };
    
    LFOWaveform waveform;
    
    PhiDial rateDial, waveDial, shapeDial;
    
    void updateWaveform() {
        waveform.set(
            rateDial.getValue(),
            (LFO::Wave)floor(waveDial.getValue()),
            shapeDial.getValue() * 0.01f
        );
    }
};
