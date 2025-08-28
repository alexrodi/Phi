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
        .minimumSize = {200, 167},
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
        ImpulseWaveform() {
            setAA(8);
        }
        
        void set(float newShape)
        {
            newShape *= 0.95f;
            shape = pow(newShape, 0.2f);
            scaleFactor = pow(newShape, 10.0f) * 200.0f + 30.0f;
            updateWavefom();
        }

    protected:
        float getSample(float phase) const override
        {
            if (phase == 0.0f) return 0.0f;
            
            return std::abs(processImpulse(phase * scaleFactor, shape)) * yScale;
        }
        
        /**
         * @brief Overrides post-processing to mirror the path vertically.
         */
        void postProcessPath() override
        {
            PhiWaveform::postProcessPath();
            
            auto bottomPath = path;
            bottomPath.applyTransform(juce::AffineTransform::verticalFlip((float)getHeight()));
            path.addPath(bottomPath);
        }
        
    private:
        const float yScale = 1.2f;
        float shape = 0.5f, scaleFactor = 1.0f;
    };

    ImpulseWaveform waveform;
    
    PhiDial frequencyDial, shapeDial;
    
    void updateWaveform() {
        waveform.set(shapeDial.getValue() * 0.01f);
    }
};
