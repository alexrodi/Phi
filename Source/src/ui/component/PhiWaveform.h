/*
  ==============================================================================

    PhiWaveform.h
    Created: 25 Aug 2025 1:52:29am
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

/**
 * @class WaveformComponent
 * @brief An abstract base class that generates a waveform path via an iterative callback.
 *
 * This class implements the Template Method pattern. It provides a `updateWavefom`
 * method that contains the main loop for iterating across the component's width.
 * For each step in the loop, it calls the pure virtual `getSample()` method, which
 * must be implemented by derived classes to return the waveform's value at that phase.
 */
struct PhiWaveform : juce::Component
{
    PhiWaveform()
    {
        setBufferedToImage(true);
        setPaintingIsUnclipped(true);
    }

    virtual ~PhiWaveform() = default;

    void paint(juce::Graphics& g) override
    {
        g.setColour(fillColour);
        g.fillRect(getLocalBounds());
        
        g.setColour(strokeColour);
        g.strokePath(path, juce::PathStrokeType(strokeWidth));
    }
    
    void resized() override {
        waveformBounds = getLocalBounds().toFloat().reduced(strokeWidth + 1.5f);
        setAA(aaValue);
    }
    
    void colourChanged() override
    {
        strokeColour = findColour(juce::Slider::thumbColourId);
        fillColour = findColour(juce::Slider::rotarySliderOutlineColourId);
    }
    
    void setAA(int aa) {
        jassert(aa > 0);
        
        aaValue = aa;
        aaFactor = 1.0f / (float)aaValue;
        aaIncrement = (pixelsPerPoint / waveformBounds.getWidth()) * aaFactor;
    }

protected:
    /**
     * @brief The iterative callback that derived classes must implement.
     * @param normalizedPhase A value from 0.0 to 1.0 representing the horizontal position.
     * @return The waveform sample for that phase, expected in the range [-1.0, 1.0].
     */
    virtual float getSample(float normalizedPhase) const = 0;

    /**
     * @brief The template method that builds the path.
     *
     * This function contains the common logic for iterating across the component's width,
     * calling the `getSample` callback for each point, and adding it to the path.
     *
     * Derived classes can override this method for custom path generation.
     */
    virtual void updateWavefom()
    {
        path.clear();
        
        float centreY = waveformBounds.getCentreY();
        float yRange = waveformBounds.getHeight() * -0.5f;
        
        // Get first value without AA
        path.startNewSubPath(waveformBounds.getX(), getSample(0.0f) * yRange + centreY);
        
        for (float x = waveformBounds.getX(); x < waveformBounds.getRight(); x += pixelsPerPoint)
        {
            // Calculate a normalized phase (0.0 to 1.0) based on the x position.
            float phase = (x - waveformBounds.getX()) / waveformBounds.getWidth();
            
            // Get the sample value(s) from the derived class's implementation.
            float sample = 0.0f;
            for(int i = 0; i < aaValue; ++i) {
                sample += getSample(phase);
                phase += aaIncrement;
            }
            
            // Apply AA average denominator
            sample *= aaFactor;
            
            // Map the sample from [-1, 1] to the component's vertical coordinates.
            const float y = sample * yRange + centreY;
            
            path.lineTo(x, y);
        }
        
        postProcessPath();
        repaint();
    }
    
    /**
     * @brief A hook for optional post-processing of the path.
     *
     * Called at the end of `updateWavefom`. The default implementation rounds the
     * corners, but derived classes can override it for custom behaviors like mirroring.
     */
    virtual void postProcessPath()
    {
        path = path.createPathWithRoundedCorners(pixelsPerPoint);
    }
    
    juce::Path path;
    juce::Colour strokeColour { juce::Colours::cyan };
    juce::Colour fillColour   { juce::Colours::transparentBlack };
    juce::Rectangle<float> waveformBounds;
    
    const float strokeWidth = 1.0f;
    const float pixelsPerPoint = 2.0f;
    int aaValue = 1; // default - No Anti-aliasing
    float aaFactor = 1.0f / (float)aaValue;
    float aaIncrement = 1.0f;
};
