/*
  ==============================================================================

    phi_Dial.h
    Created: 9 Feb 2020 11:02:38pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class ui_Dial    : public Slider
{
public:
    ui_Dial(float rangeLow, float rangeHigh, std::string valueSuffix, int decimals, Slider::Listener* listener);
    ~ui_Dial();

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ui_Dial)
};