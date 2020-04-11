/*
  ==============================================================================

    Module.cpp
    Created: 29 Mar 2020 11:38:39am
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

///@cond
#include <JuceHeader.h>
///@endcond
#include "Module.h"

//==============================================================================

Module::Module(Props arguments) :
props{arguments}
{
    for (int i=0; i<arguments.inletNumber; i++)
        addAndMakeVisible( inlets.add( new phi_Inlet("inlet test name") ) );
    
    for (int i=0; i<arguments.outletNumber; i++)
        addAndMakeVisible( outlets.add( new phi_Outlet("outlet test name") ) );

    setPaintingIsUnclipped(true);
        
    setPlayConfigDetails (arguments.inletNumber, arguments.outletNumber, getSampleRate(), getBlockSize());
}

void Module::resized()
{
    Rectangle<int> moduleBounds = getLocalBounds();
    moduleBounds = placeInletsOutlets( moduleBounds );
    
    wasResized(moduleBounds);
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
