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
#include "../../ui/component/PhiWaveform.h"
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
        .defaultSize = {220, 220},
        .minimumSize = {100, 100},
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

private:
    struct ImpulseWaveform : public PhiWaveform
    {
    public:
        void set(const float newShape)
        {
            shape = newShape;
            phaseScale = pow(shape, 50.0f) * 200.0f + 30.0f;
            aaIncrement = (pixelsPerPoint / waveformBounds.getWidth()) * aaFactor * phaseScale;
            updateWavefom();
        }

    protected:
        float getSample(float phase) const override
        {
            if (phase == 0.0f) return 0.0f;
            
            float impulsePhase = phase * phaseScale;
            
            float sample = 0.0f;
            for(int i = 0; i < aaValue; ++i)
            {
                sample += std::abs(processImpulse(impulsePhase, shape));
                impulsePhase += aaIncrement;
            }
            
            return sample * aaFactor * yScale;
        }
        
        /**
         * @brief Overrides post-processing to mirror the path vertically.
         */
        void postProcessPath() override
        {
            path = path.createPathWithRoundedCorners((float)pixelsPerPoint);
            auto bottomPath = path;
            bottomPath.applyTransform(juce::AffineTransform::verticalFlip((float)getHeight()));
            path.addPath(bottomPath);
        }
        
    private:
        const float yScale = 1.2f;
        const int aaValue = 8; // x8 anti-aliasing
        float aaFactor = 1.0f / (float)aaValue;
        float shape = 0.5f, aaIncrement = 1.0f, phaseScale = 1.0f;
    };

    ImpulseWaveform waveform;
    
    PhiDial frequencyDial, shapeDial;
    
    void updateWaveform() {
        waveform.set(pow(shapeDial.getValue() * 0.0095f, 0.2f));
    }
};
