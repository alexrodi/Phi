/*
  ==============================================================================

    StringUI.cpp
    Created: 25 Mar 2020 3:14:32pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#include "StringUI.h"


StringUI::StringUI(ModuleProcessor& processor) :
ModuleUI({
            // All modules must initialize these properties
            .name =  "String",
            .inlets = {"In 1", "In 2"},
            .outlets = {"Out 1", "Out 2"},
            .width = 330,
            .height = 160,
            .minimumHeight = 100,
            .processor = processor
        }),
frequencyDial( "Frequency" , 20.0f, 10000.0f, 0.3f, " Hz"),
positionDial(  "Position"  , 0.0f , 1.0f    , 1.0f, " %",  0),
dampDial(      "Damping"   , 0.0f , 1.0f    , 1.0f, " %",  0),
decayDial(     "Decay"     , 0.0f , 1.0f    , 1.0f, " %",  0),
modeButton(    "Mode", ui_SliderButton::Left)
{
    auto percentageTextFromValueCB = [] (float f) -> String { return String(int(f * 100)); };
    auto percentageValueFromTextCB = [] (String s) -> float { return float(s.toUTF8().getDoubleValue()) * 0.01; };
    
    positionDial.textFromValueFunction = percentageTextFromValueCB;
    dampDial.textFromValueFunction = percentageTextFromValueCB;
    decayDial.textFromValueFunction = percentageTextFromValueCB;
    positionDial.valueFromTextFunction = percentageValueFromTextCB;
    dampDial.valueFromTextFunction = percentageValueFromTextCB;
    decayDial.valueFromTextFunction = percentageValueFromTextCB;
    
    addAndMakeVisible(frequencyDial);
    addAndMakeVisible(positionDial);
    addAndMakeVisible(dampDial);
    addAndMakeVisible(decayDial);
    addAndMakeVisible(modeButton);
    
    modeButton.addListener(this);
    
    frequencyDial.setValue(440.0f);
}

StringUI::~StringUI()
{
}

void StringUI::paint (Graphics&)
{
}

void StringUI::wasResized(Rectangle<int> moduleBounds)
{
    const int dialWidth = (float)moduleBounds.getWidth()/4.0f;
    
    auto buttonRow = moduleBounds.removeFromTop(30);
    
    modeButton.setBounds(buttonRow.removeFromLeft(70));
    
    frequencyDial.setBounds( moduleBounds.removeFromLeft(dialWidth) );
    positionDial.setBounds( moduleBounds.removeFromLeft(dialWidth) );
    dampDial.setBounds( moduleBounds.removeFromLeft(dialWidth) );
    decayDial.setBounds( moduleBounds.removeFromLeft(dialWidth) );
}


void StringUI::sliderValueChanged (Slider* slider)
{
    auto& params = props.processor.params;
    
    if (slider == &frequencyDial)
        *params.getRawParameterValue("freq") = slider->getValue();
    else if (slider == &positionDial)
        *params.getRawParameterValue("pos") = slider->getValue();
    else if (slider == &dampDial)
        *params.getRawParameterValue("damp") = slider->getValue();
    else if (slider == &decayDial)
        *params.getRawParameterValue("decay") = slider->getValue();
}

void StringUI::buttonClicked (Button* button)
{
    if (button == &modeButton)
        *props.processor.params.getRawParameterValue("mode") = (float)button->getToggleState();
}
