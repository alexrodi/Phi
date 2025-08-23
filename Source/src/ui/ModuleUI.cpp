/*
  ==============================================================================

    ModuleUI.cpp
    Created: 21 Apr 2020 7:39:35pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#include "ModuleUI.h"
#include "../ModuleProcessor.h"

//==============================================================================

ModuleUI::ModuleUI(Props arguments) :
AudioProcessorEditor(arguments.processor),
props(arguments)
{
    // Add all inlets and outlets as children and make them visible
    inlets.reserve(props.inlets.size());
    for (auto& inletName : props.inlets) {
        inlets.push_back( InletUI{inletName} );
        addAndMakeVisible( inlets.back() );
    }
    
    outlets.reserve(props.outlets.size());
    for (auto& outletName : props.outlets) {
        outlets.push_back( OutletUI{outletName} );
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

void ModuleUI::setShowPortLabels(ShowPortLabels show) {
    for (auto& port : inlets)
        port.showLabel(show);
    
    for (auto& port : outlets)
        port.showLabel(show);
}

void ModuleUI::placePorts(std::vector<PortUI>& ports, juce::Rectangle<int> portBounds)
{
    const int portHeight = portBounds.getHeight() / (int)ports.size();
    
    // Divide the space for each Port
    for (auto& port : ports)
        port.setBounds( portBounds.removeFromTop(portHeight) );
}


juce::Rectangle<int> ModuleUI::placeInletsAndOutlets(juce::Rectangle<int> moduleBounds)
{
    if (!inlets.empty())
        placePorts(inlets, moduleBounds.removeFromLeft(PLUG_COLUMN_WIDTH));
    
    if (!outlets.empty())
        placePorts(outlets, moduleBounds.removeFromRight(PLUG_COLUMN_WIDTH));
    
    return moduleBounds;
}
