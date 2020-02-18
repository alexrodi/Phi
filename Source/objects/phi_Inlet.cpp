/*
  ==============================================================================

    phi_Inlet.cpp
    Created: 18 Feb 2020 11:19:12pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#include <JuceHeader.h>
#include "phi_Inlet.h"

//==============================================================================
phi_Inlet::phi_Inlet()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
}

phi_Inlet::~phi_Inlet()
{
}

void phi_Inlet::paint (Graphics& g)
{
    
    g.setColour(Colours::grey);
    g.fillEllipse(viewport);
    g.setColour(Colours::darkgrey);
    g.drawEllipse(viewport, 3);
}

void phi_Inlet::resized()
{
    viewport = getLocalBounds().withSizeKeepingCentre(12, 12).toFloat();
}
