/*
  ==============================================================================

    phi_Dial.cpp
    Created: 9 Feb 2020 11:02:38pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#include <JuceHeader.h>
#include "phi_Dial.h"

phi_Dial::phi_Dial(float rangeLow, float rangeHigh, std::string valueSuffix, int decimals, Slider::Listener* listener)
{
    setRange                     (rangeLow, rangeHigh);
    setTextValueSuffix           (valueSuffix);
    addListener                  (listener);
    setSliderStyle               (Slider::RotaryVerticalDrag);
    setTextBoxStyle              (Slider::TextBoxBelow, true, 80, 20);
    setNumDecimalPlacesToDisplay (decimals);
    setTextBoxIsEditable         (true);
    
    setPaintingIsUnclipped (true);
}

 phi_Dial::~phi_Dial()
{
}
