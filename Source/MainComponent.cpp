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
    
    setLookAndFeel(&lookandfeel);
    lookandfeel.setColour(PopupMenu::backgroundColourId, Colours::darkgrey.darker());
    lookandfeel.setColour(PopupMenu::highlightedBackgroundColourId, Colour::greyLevel(0.2));
    
    sendLookAndFeelChange();
}

MainComponent::~MainComponent()
{
    setLookAndFeel(nullptr);
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
        
        mainMenu.setLookAndFeel(&lookandfeel);
        
        const int result = mainMenu.show();
        
        if (result){
            ModuleBox* module = new ModuleBox(&selectedModules);
            modules.add(module);
            addAndMakeVisible(module);
            module->setBounds(e.x, e.y, 350, 200);
        }
    }
}
