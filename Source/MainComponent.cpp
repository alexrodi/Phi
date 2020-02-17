/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    setSize (600, 400);
    
    
}

MainComponent::~MainComponent()
{
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    g.fillAll (Colours::darkgrey);
}

void MainComponent::resized()
{
}

void MainComponent::mouseDown(const MouseEvent& e)
{
    selectedModules.deselectAll();
    
    if (e.mods.isRightButtonDown()){
        PopupMenu subMenu;
        subMenu.addItem (1, "Oscillator");

        PopupMenu mainMenu;
        mainMenu.addSubMenu ("Add Module...", subMenu);

        const int result = mainMenu.show();
        
        if (result){
            ModuleBox* module = new ModuleBox(&selectedModules);
            modules.add(module);
            addAndMakeVisible(module);
            module->setBounds(e.x, e.y, 350, 200);
        }
    }
}
