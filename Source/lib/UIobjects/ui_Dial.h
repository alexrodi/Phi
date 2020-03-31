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
    ui_Dial(float, float, std::string, int, Slider::Listener*);
    ~ui_Dial();
    
    void paint (Graphics& g) override;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ui_Dial)
};
