/*
  ==============================================================================

    StringUI.h
    Created: 25 Mar 2020 3:14:32pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

#include "../../ModuleProcessor.h"
#include "../../ui/component/PhiDial.h"
#include "../../ui/component/PhiSliderButton.h"

class StringUI    : public ModuleUI
{
public:
    StringUI(ModuleProcessor& processor) :
    ModuleUI({
        // All modules must initialize these properties
        .name =  "String",
        .inlets = {"In", "Freq", "Pos", "Damp", "Decay"},
        .outlets = {"Out"},
        .width = 330,
        .height = 160,
        .minimumHeight = 100,
        .processor = processor
    }),
    frequencyDial(*processor.params.getParameter("freq")),
    positionDial(*processor.params.getParameter("pos")),
    dampDial(*processor.params.getParameter("damp")),
    decayDial(*processor.params.getParameter("decay")),
    modeButton("Mode", PhiSliderButton::LabelPosition::Left),
    modeAttachment(*processor.params.getParameter("mode"), modeButton)
    {
        addAndMakeVisible(frequencyDial);
        addAndMakeVisible(positionDial);
        addAndMakeVisible(dampDial);
        addAndMakeVisible(decayDial);
        addAndMakeVisible(modeButton);
    }
    
    ~StringUI() {}

    void paint (juce::Graphics&) override {}
    
    void onResize(juce::Rectangle<int> moduleBounds) override
    {
        int dialWidth = moduleBounds.getWidth() / 4;
        
        auto buttonRow = moduleBounds.removeFromTop(30);
        
        modeButton.setBounds(buttonRow.removeFromLeft(70));
        
        frequencyDial.setBounds( moduleBounds.removeFromLeft(dialWidth) );
        positionDial.setBounds( moduleBounds.removeFromLeft(dialWidth) );
        dampDial.setBounds( moduleBounds.removeFromLeft(dialWidth) );
        decayDial.setBounds( moduleBounds.removeFromLeft(dialWidth) );
    }

private:
    PhiDial frequencyDial, positionDial, dampDial, decayDial;
    PhiSliderButton modeButton;
    juce::ButtonParameterAttachment modeAttachment;
};
