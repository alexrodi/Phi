/*
  ==============================================================================

    ModuleUI.cpp
    Created: 21 Apr 2020 7:39:35pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

///@cond
#include <JuceHeader.h>
///@endcond
#include "ModuleUI.h"

//==============================================================================

ModuleUI::ModuleUI(Props arguments) :
AudioProcessorEditor(arguments.processor),
props{arguments}
{
    // Add all inlets and outlets as children and make them visible
    for (String inletName : props.inlets)
        addAndMakeVisible( inlets.add( new Inlet(inletName) ) );
    
    for (String outletName : props.outlets)
        addAndMakeVisible( outlets.add( new Outlet(outletName) ) );
        
    setPaintingIsUnclipped(true);
}

void ModuleUI::resized()
{
    auto moduleBounds = getLocalBounds();
    moduleBounds = placeInletsAndOutlets( moduleBounds );
    
    // Call the function to resize the module
    wasResized(moduleBounds);
}

void ModuleUI::placePlugs(OwnedArray<Plug>& plugArray, Rectangle<int> plugBounds)
{
    const int plugHeight = (float)plugBounds.getHeight()/(float)plugArray.size();
    
    if (! plugArray.isEmpty())
    {
        // Divide the space to each plug
        for (auto plug : plugArray)
            plug->setBounds( plugBounds.removeFromTop(plugHeight) );
    }
}


const Rectangle<int> ModuleUI::placeInletsAndOutlets(Rectangle<int> moduleBounds)
{
    placePlugs(inlets, moduleBounds.removeFromLeft(30));
    placePlugs(outlets, moduleBounds.removeFromRight(30));
    
    return moduleBounds;
}
