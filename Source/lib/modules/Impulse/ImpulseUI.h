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
#include "ImpulseProcessor.h"
#include "../../UIobjects/ui_Dial.h"

//==============================================================================
/*
*/
class ImpulseUI    : public ModuleUI,
                     public Slider::Listener
{
public:
    ImpulseUI(ImpulseProcessor&);
    ~ImpulseUI();

    void paint (Graphics&) override;
    void wasResized(Rectangle<int>) override;
    
    void mouseDown(const MouseEvent&) override;
    
    void lookAndFeelChanged() override;

private:
    
    ImpulseProcessor& processor;
    
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
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ImpulseUI)
};
