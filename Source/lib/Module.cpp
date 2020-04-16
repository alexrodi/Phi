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
props{arguments},
inletNumber{props.inlets.size()},
outletNumber{props.outlets.size()}
{
    for (String inletName : props.inlets)
        addAndMakeVisible( inlets.add( new Inlet(inletName) ) );
    
    for (String outletName : props.outlets)
        addAndMakeVisible( outlets.add( new Outlet(outletName) ) );
        
    setPlayConfigDetails (props.inlets.size(), props.outlets.size(), getSampleRate(), getBlockSize());
        
    setPaintingIsUnclipped(true);
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
    for (Inlet* inlet : inlets)
        inlet->setBounds( inletBounds.removeFromTop((float)moduleBounds.getHeight()/(float)inlets.size()) );

    // Place the outlets (equidistant)
    Rectangle<int> outletBounds = moduleBounds.removeFromRight(30);
    for (Outlet* outlet : outlets)
        outlet->setBounds( outletBounds.removeFromTop((float)moduleBounds.getHeight()/(float)outlets.size()) );
    
    return moduleBounds;
}
