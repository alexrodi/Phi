/*
  ==============================================================================

    StringUI.h
    Created: 25 Mar 2020 3:14:32pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

#include "../../ui/ModuleUI.h"
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
        .width = 210,
        .height = 260,
        .minimumHeight = 100,
        .processor = processor
    }),
    frequencyDial(*processor.params.getParameter("freq")),
    positionDial(*processor.params.getParameter("pos")),
    dampDial(*processor.params.getParameter("damp")),
    decayDial(*processor.params.getParameter("decay")),
    modeButton("A", "B"),
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
    
    void resized() override
    {
        auto bounds = getLocalBounds();
        
        modeButton.setBounds(bounds.removeFromBottom(30).withSizeKeepingCentre(70, 30));
        
        int rowPadding = 10;
        int rowHeight = (bounds.getHeight() - rowPadding) / 2;
        
        auto topRow = bounds.removeFromTop(rowHeight);
        frequencyDial.setBounds( topRow.removeFromLeft(topRow.getWidth() / 2) );
        decayDial.setBounds( topRow );
        
        auto bottomRow = bounds.removeFromBottom(rowHeight);
        positionDial.setBounds( bottomRow.removeFromLeft(bottomRow.getWidth() / 2) );
        dampDial.setBounds( bottomRow );
    }

private:
    PhiDial frequencyDial, positionDial, dampDial, decayDial;
    PhiSliderButton modeButton;
    juce::ButtonParameterAttachment modeAttachment;
};
