/*
  ==============================================================================

    ui_ToggleButton.h
    Created: 16 Apr 2020 12:33:07am
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class ui_ToggleButton : public Button
{
public:
    ui_ToggleButton() :
    Button("")
    {
        setPaintingIsUnclipped(true);
        setClickingTogglesState(true);
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
        g.setColour(findColour(Slider::thumbColourId));
        g.fillRect(buttonRect.reduced(1));
    }
    
    void resized () override
    {
        buttonRect = getLocalBounds().reduced(1).toFloat();
    }
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ui_ToggleButton)
};
