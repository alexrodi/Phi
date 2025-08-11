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
#include "ModuleProcessor.h"

//==============================================================================

ModuleUI::ModuleUI(Props arguments) :
AudioProcessorEditor(arguments.processor),
props(arguments)
{
    // Add all inlets and outlets as children and make them visible
    inlets.reserve(props.inlets.size());
    for (auto& inletName : props.inlets) {
        inlets.push_back( InletPlug{inletName} );
        addAndMakeVisible( inlets.back() );
    }
    
    outlets.reserve(props.outlets.size());
    for (auto& outletName : props.outlets) {
        outlets.push_back( OutletPlug{outletName} );
        addAndMakeVisible( outlets.back() );
    }
        
    setPaintingIsUnclipped(true);
    setBufferedToImage(true);
}

ModuleUI::~ModuleUI()
{
    props.processor.editorBeingDeleted(this);
}

void ModuleUI::resized()
{
    auto moduleBounds = getLocalBounds();
    moduleBounds = placeInletsAndOutlets( moduleBounds );
    
    // Call the function to resize the module
    onResize(moduleBounds);
}

void ModuleUI::placePlugs(std::vector<Plug>& plugs, Rectangle<int> plugBounds)
{
    const int plugHeight = plugBounds.getHeight() / (int)plugs.size();
    
    // Divide the space for each plug
    for (auto& plug : plugs)
        plug.setBounds( plugBounds.removeFromTop(plugHeight) );
}


Rectangle<int> ModuleUI::placeInletsAndOutlets(Rectangle<int> moduleBounds)
{
    if (!inlets.empty())
        placePlugs(inlets, moduleBounds.removeFromLeft(PLUG_COLUMN_WIDTH));
    
    if (!outlets.empty())
        placePlugs(outlets, moduleBounds.removeFromRight(PLUG_COLUMN_WIDTH));
    
    return moduleBounds;
}
