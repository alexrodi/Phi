/*
  ==============================================================================

    PhiToggleButton.h
    Created: 16 Apr 2020 12:33:07am
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

///@cond
#include <JuceHeader.h>
///@endcond

//==============================================================================
/*
*/
class PhiButton : public Button
{
public:
    enum Mode {
        Toggle,
        Momentary
    };
    
    PhiButton(Mode mode) :
    Button("")
    {
        setPaintingIsUnclipped(true);
        setClickingTogglesState(mode == Mode::Toggle);
    }
private:
    Rectangle<float> buttonRect;
    // Override the button's paint method
    void paintButton (Graphics& g,
                      bool shouldDrawButtonAsHighlighted,
                      bool shouldDrawButtonAsDown) override
    {
        g.setColour(Colours::grey);
        g.drawRect(buttonRect, 1);
        if (getToggleState())
        {
            g.setColour(findColour(Slider::thumbColourId));
            g.fillRect(buttonRect.reduced(1));
        }
        
    }
    
    void mouseDown (const MouseEvent& e) override
    {
        if (!getClickingTogglesState())
            setToggleState(true, NotificationType::sendNotificationSync);
    }
    
    void mouseUp (const MouseEvent& e) override
    {
        if (!getClickingTogglesState())
            setToggleState(false, NotificationType::sendNotificationSync);
    }
    
    void resized () override
    {
        buttonRect = getLocalBounds().reduced(1).toFloat();
    }
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PhiButton)
};
