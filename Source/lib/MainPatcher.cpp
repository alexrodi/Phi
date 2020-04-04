/*
  ==============================================================================

    MainPatcher.cpp
    Created: 29 Mar 2020 12:51:46am
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#include <JuceHeader.h>
#include "MainPatcher.h"

//==============================================================================
MainPatcher::MainPatcher()
{
    // Menu and submenus content
    // Submenus must be filled before the main
    modulesSubMenu.addItem (1, "Impulse");
    modulesSubMenu.addItem (2, "Gain");
    rightClickMenu.addSubMenu ("Add Module...", modulesSubMenu);
    
    addAndMakeVisible(connections);
    
    setSize(1000, 1000);
}

MainPatcher::~MainPatcher()
{
    AudioProcessorGraph::clear();
    rightClickMenu.dismissAllActiveMenus();
    modulesSubMenu.dismissAllActiveMenus();
}

//==============================================================================
void MainPatcher::prepareToPlay (double sampleRate, int maximumExpectedSamplesPerBlock)
{
   
}

void MainPatcher::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    
}

void MainPatcher::releaseResources()
{
    
}

//==============================================================================
void MainPatcher::paint (Graphics& g)
{
}

void MainPatcher::resized()
{
    connections.setBounds(getLocalBounds());
}

void MainPatcher::mouseDown(const MouseEvent& e)
{
    // Deselect all modules when clicking the window
    selectedModules.deselectAll();
    
    // Right Click
    if (e.mods.isRightButtonDown()){
        
        // Displays the menu and returns the ID of the selected item (0 if clicked outside)
        const int result = rightClickMenu.showMenu(PopupMenu::Options().withParentComponent(getParentComponent()));
        
        // There's only one possible result for now (1 - Impulse)
        if (result==1)
        {
            createModule<module_Impulse>(e.position);
        }
        else if (result==2)
        {
            createModule<module_Gain>(e.position);
        }
    }
}

void MainPatcher::togglePatchCordType()
{
    connections.togglePatchCordType();
}

// Registers all inlets and outlets with the connections component
void MainPatcher::registerInletsAndOutlets(std::unique_ptr<Module>& module) {
    
    OwnedArray<phi_Inlet>& inlets = module->inlets;
    for (phi_Inlet* inlet : inlets)
    {
        inlet->inletID = connections.registerInlet(inlet);
        inlet->addActionListener(&connections);
    }
    
    OwnedArray<phi_Outlet>& outlets = module->outlets;
    for (phi_Outlet* outlet : outlets)
    {
        outlet->outletID = connections.registerOutlet(outlet);
        outlet->addActionListener(&connections);
    }
    
}

template <class moduleClass>
void MainPatcher::createModule(Point<float> position)
{
    ModuleBox* moduleBox;
    
    std::unique_ptr<Module> newModule = std::make_unique<moduleClass>();
        
    moduleBox = new ModuleBox(newModule.get(), selectedModules);
    
    modules.add(moduleBox);

    registerInletsAndOutlets(newModule);
    
    // Display and set its position
    addAndMakeVisible(moduleBox);
    moduleBox->setTopLeftPosition(position.toInt());
    moduleBox->addActionListener(&connections);
    
    addNode(std::move(newModule));
}
