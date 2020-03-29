/*
  ==============================================================================

    Module.cpp
    Created: 29 Mar 2020 11:38:39am
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#include <JuceHeader.h>
#include "Module.h"

//==============================================================================

Module::Module(Arguments arguments) :
name{arguments.name},
width{arguments.width},
height{arguments.height}
{
    for (int i=0; i<arguments.inletNumber; i++)
        addAndMakeVisible( inlets.add( new phi_Inlet() ) );
    
    for (int i=0; i<arguments.outletNumber; i++)
        addAndMakeVisible( outlets.add( new phi_Outlet() ) );

    setPaintingIsUnclipped(true);
}


const Rectangle<int> Module::placeInletsOutlets(Rectangle<int> moduleBounds)
{
    // Place the inlets (equidistant)
    Rectangle<int> inletBounds = moduleBounds.removeFromLeft(30);
    for (phi_Inlet* inlet : inlets)
        inlet->setBounds( inletBounds.removeFromTop((float)moduleBounds.getHeight()/(float)inlets.size()) );

    // Place the outlets (equidistant)
    Rectangle<int> outletBounds = moduleBounds.removeFromRight(30);
    for (phi_Outlet* outlet : outlets)
        outlet->setBounds( outletBounds.removeFromTop((float)moduleBounds.getHeight()/(float)outlets.size()) );
    
    return moduleBounds;
}