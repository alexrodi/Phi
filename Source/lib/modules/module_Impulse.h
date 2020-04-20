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
    
    void lookAndFeelChanged() override;

private:
    
    float currentPhase = 0;
    double sampleRate = 44100;
    
    //Dials
    ui_Dial frequencyDial;
    ui_Dial shapeDial;
    
    //Waveform
    class Waveform
    {
        const float strokeWidth = 2;
        Rectangle<float> viewPort;
        float yRange;
        float centreY;
        Colour colour;
        Path topPath;
        Path bottomPath;
        ColourGradient colourGradient;
        const void updateColour();
    public:
        const void setColour(const Colour&);
        const void setViewPort(const Rectangle<float>);
        const void updateForm(const float);
        const void draw(Graphics&);
    } waveForm;
    
    //Listeners
    void sliderValueChanged (Slider*) override;
    void frequencyDialChanged (float value);
    void shapeDialChanged (float value);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (module_Impulse)
};
