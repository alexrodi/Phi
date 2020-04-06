/*
  ==============================================================================

    MainPatcher.h
    Created: 29 Mar 2020 12:51:46am
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ModuleBox.h"
#include "Connections.h"
#include "modules/Modules.h"

//==============================================================================
/*
*/
class MainPatcher    : public Component,
                       public DragAndDropContainer,
                       public ChangeListener
{
public:
    MainPatcher();
    ~MainPatcher();

    void paint (Graphics&) override;
    void resized() override;
    
    void togglePatchCordType();
    
    std::unique_ptr<AudioProcessorGraph> mainProcessor;
    
private:
    
    AudioDeviceManager deviceManager;
    AudioProcessorPlayer player;
    
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
    
    
    void registerInletsAndOutlets(Module*, uint32);
    
    template <class moduleClass>
    void createModule(Point<float>);
    
    void initialiseGraph();
    
    void mouseDown(const MouseEvent& e) override;
    
    void changeListenerCallback (ChangeBroadcaster* source) override;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainPatcher)
};
