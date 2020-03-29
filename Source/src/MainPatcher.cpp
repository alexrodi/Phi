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
    rightClickMenu.addSubMenu ("Add Module...", modulesSubMenu);
    
    addAndMakeVisible(connections);
    
    setSize(1000, 1000);
}

MainPatcher::~MainPatcher()
{
    rightClickMenu.dismissAllActiveMenus();
    modulesSubMenu.dismissAllActiveMenus();
}

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
        if (result)
        {
            createModule(e.position);
        }
    }
}

// Registers all inlets and outlets with the connections component
void MainPatcher::registerInletsAndOutlets(ModuleBox *module) {
    
    OwnedArray<phi_Inlet>& inlets = module->module->inlets;
    for (phi_Inlet* inlet : inlets)
    {
        inlet->inletID = connections.registerInlet(inlet);
        inlet->addActionListener(&connections);
    }
    
    OwnedArray<phi_Outlet>& outlets = module->module->outlets;
    for (phi_Outlet* outlet : outlets)
    {
        outlet->outletID = connections.registerOutlet(outlet);
        outlet->addActionListener(&connections);
    }
    
}

void MainPatcher::createModule(Point<float> position)
{
    // Create the module
    ModuleBox* module = new ModuleBox(new Impulse(), selectedModules);

    registerInletsAndOutlets(module);

    // Add it to the array
    modules.add(module);

    // Display and set its (initial) bounds
    addAndMakeVisible(module);
    module->setTopLeftPosition(position.toInt());
    module->addActionListener(&connections);
}
