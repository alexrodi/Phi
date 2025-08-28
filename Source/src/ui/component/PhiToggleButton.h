/*
  ==============================================================================

    PhiToggleButton.h
    Created: 16 Apr 2020 12:33:07am
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once


//==============================================================================
/*
*/
class PhiToggleButton : public juce::Button
{
public:
    PhiToggleButton() :
    Button("")
    {
        setPaintingIsUnclipped(true);
        setClickingTogglesState(true);
    }
private:
    juce::Rectangle<float> buttonRect;
    // Override the button's paint method
    void paintButton (juce::Graphics& g,
                      bool shouldDrawButtonAsHighlighted,
                      bool shouldDrawButtonAsDown) override
    {
        g.setColour(findColour(PhiColourIds::Module::Outline));
        g.drawRect(buttonRect, 1);
        g.setColour(findColour(PhiColourIds::Module::Highlight));
        g.fillRect(buttonRect.reduced(1));
    }
    
    void resized () override
    {
        buttonRect = getLocalBounds().reduced(1).toFloat();
    }
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PhiToggleButton)
};
