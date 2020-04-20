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
#include "../Module.h"
#include "../UIobjects/ui_Dial.h"

//==============================================================================
/*
*/
class module_Impulse    : public Module,
                          public Slider::Listener
{
public:
    module_Impulse();
    ~module_Impulse();
    
    // Audio =======================================================================
    void prepareToPlay (double, int) override;
    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;
    void releaseResources() override;

    void paint (Graphics&) override;
    void wasResized(Rectangle<int>) override;
    
    void mouseDown(const MouseEvent&) override;
    
    void lookAndFeelChanged() override;
    
    void triggerImpulse();

private:
    
    float currentPhase = 0;
    double sampleRate = 44100;
    
    static float processImpulse(float, float);
    
    //Dials
    ui_Dial frequencyDial;
    ui_Dial shapeDial;
    
    //Waveform
    class Waveform : public Component
    {
    public:
        const void setColour(const Colour&);
        const void updateForm(const float);
        void resized() override;
        void paint(Graphics&) override;
    private:
        const float strokeWidth = 2;
        float yRange;
        float centreY;
        Colour colour;
        Path topPath;
        Path bottomPath;
        ColourGradient colourGradient;
        const void updateColour();
    } waveForm;
    
    //Listeners
    void sliderValueChanged (Slider*) override;
    void frequencyDialChanged (float value);
    void shapeDialChanged (float value);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (module_Impulse)
};
