/*
  ==============================================================================

    MainPatcher.h
    Created: 29 Mar 2020 12:51:46am
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

///@cond
#include <JuceHeader.h>
///@endcond
#include "ModuleBox.h"
#include "Connections.h"
#include "AudioEngine.h"

//==============================================================================
/// The main view of Phi, this class handles all the module creation, hosting and patching
class MainPatcher    : public Component,
                       public DragAndDropContainer,
                       public ChangeListener
{
public:
    MainPatcher(std::unique_ptr<AudioEngine>);
    ~MainPatcher();

    void paint (Graphics&) override;
    void resized() override;
    
    void mouseDown(const MouseEvent& e) override;
    
    /// Toggles between two patch-cord drawing routines
    void togglePatchCordType(bool);
    
    /// Toggles inlet/outlet name display type
    void toggleInoutType(bool);
    
    /// Listener for key presses
    bool keyPressed (const KeyPress& key) override;
    
private:
    /// A pointer to the AudioEngine to add processors (modules) and make connections
    std::unique_ptr<AudioEngine> audioEngine;
    
    /// An OwnedArray for storing and accessing all the modules (ModuleBox's) in the patcher
    OwnedArray<ModuleBox> modules;
    
    /// The list of selected modules, it gets passed to each module because they subscribe themselves to the list, they also use it for dragging behaviour
    SelectedItemSet<ModuleBox*> selectedModules;
    
    /// The graphical component that represents connections between modules. It is drawn always on top of everything in the patcher.
    Connections connections;
    
    /// The right click menu
    PopupMenu rightClickMenu;
    /// The "Modules" sub-menu
    PopupMenu modulesSubMenu;
    
    TooltipWindow tooltipWindow;
    
    void registerPlugs(OwnedArray<Plug>&, Connections::PlugMode, uint32);
    
    /** Registers all inlets and outlets of a module with the connections component.
     This function performs three jobs for each inlet/outlet:
     registers with connections, sets the resulting registry ID in the inlet/outlet and adds connections as a listener so that it may receive actions from it. */
    void registerInletsAndOutlets(ModuleUI&);
    
    /// Creates a module of type moduleClass at certain (top-left) point in the patcher, registers it to connections and adds it to audioEngine
    template <class> void createModule(Point<float>);
    
    /// Deletes a module and all its connections from the patcher and audioEngine, unregisters it from connections
    void deleteModule(ModuleBox*);
    /// Runs deleteModule() for every selected module
    void deleteAllSelectedModules();
    
    /// We use this currently to get notified from Connections when a new connection is made, so that it may be correspondingly applied in the AudioEngine
    void changeListenerCallback (ChangeBroadcaster* source) override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainPatcher)
};
