/*
  ==============================================================================

    Gain.cpp
    Created: 29 Mar 2020 3:14:32pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#include <JuceHeader.h>
#include "module_Gain.h"

module_Gain::module_Gain() :
Module{{
    // All modules must initialize these properties
    .name =  "Gain",
    .inletNumber = 1,
    .outletNumber = 1,
    .width = 150,
    .height = 150
}},
gainDial(-70, 12, " dB", 2, this)
{

    addAndMakeVisible(gainDial);
    
}

module_Gain::~module_Gain()
{
    
}

//==============================================================================
void module_Gain::paint (Graphics& g)
{

}

void module_Gain::resized()
{

    // The local bounds
    Rectangle<int> moduleBounds = getLocalBounds();
    
    // This line is mandatory for all modules with inlets/outlets
    moduleBounds = placeInletsOutlets( moduleBounds );
    
    
    gainDial.setBounds( moduleBounds );

}

void module_Gain::sliderValueChanged (Slider* slider)
{
    if (slider == &gainDial){
        repaint();
    }
}
