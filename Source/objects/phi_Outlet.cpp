/*
  ==============================================================================

    phi_Outlet.cpp
    Created: 18 Feb 2020 11:19:27pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#include <JuceHeader.h>
#include "phi_Outlet.h"

//==============================================================================
phi_Outlet::phi_Outlet()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

phi_Outlet::~phi_Outlet()
{
}

void phi_Outlet::paint (Graphics& g)
{
    g.setColour(Colours::darkgrey);
    g.fillEllipse(viewport);
    g.setColour(Colours::grey);
    g.drawEllipse(viewport, 3);
}

void phi_Outlet::resized()
{
     viewport = getLocalBounds().withSizeKeepingCentre(12, 12).toFloat();
}
