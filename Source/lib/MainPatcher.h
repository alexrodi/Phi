/*
  ==============================================================================

    MainPatcher.h
    Created: 29 Mar 2020 12:51:46am
    Author:  Alexandre Rodrigues
 
    The MainPatcher class contains all the functionality relating to
    creating, patching and all the audio functionality of the modules.
 
    The modules are kept in an array and subscribe to a SelectedItemSet
    so they can be selected and grouped.
    
    It lives in a Viewport in the MainComponent and occupies most of the window.
    
    The audio engine is an AudioProcessorGraph where each module is a node (Audio Processor).
 
    Connections are notitified from the connections member
    and are applied to the audio in applyAudioConnections().

  ==============================================================================
*/

#pragma once

///@cond
#include <JuceHeader.h>
///@endcond
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
    
    bool keyPressed (const KeyPress& key) override;
    
private:
    
    std::unique_ptr<AudioProcessorGraph> mainProcessor;
    
    AudioDeviceManager deviceManager;
    AudioProcessorPlayer player;
    AudioProcessorGraph::Node::Ptr outputNode;
    
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
    
    template <class> void createModule(Point<float>);
    
    void deleteModule(ModuleBox*);
    void deleteAllSelectedModules();
    
    void applyAudioConnections();
    
    void mouseDown(const MouseEvent& e) override;
    
    void changeListenerCallback (ChangeBroadcaster* source) override;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainPatcher)
};
