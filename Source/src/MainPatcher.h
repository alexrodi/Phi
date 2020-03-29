/*
  ==============================================================================

    MainPatcher.h
    Created: 29 Mar 2020 12:51:46am
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../lib/ModuleBox.h"
#include "../lib/Connections.h"
#include "../lib/modules/Impulse.h"

//==============================================================================
/*
*/
class MainPatcher    : public Component
{
public:
    MainPatcher();
    ~MainPatcher();

    void paint (Graphics&) override;
    void resized() override;
    void mouseDown(const MouseEvent& e) override;

private:
    
    // The array of modules
    OwnedArray<ModuleBox> modules;
    
    // The list of selected modules
    // gets passed to each module because they subscribe themselves to the list
    // they also use it for dragging all the selected modules at once
    SelectedItemSet<ModuleBox*> selectedModules;
    
    // The connections class
    Connections connections;
    
    // Right click menu and submenus
    PopupMenu rightClickMenu;
    PopupMenu modulesSubMenu;
    
    void registerInletsAndOutlets(ModuleBox*);
    
    void createModule(Point<float>);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainPatcher)
};
