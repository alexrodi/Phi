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

//==============================================================================
/// The main view of Phi, this class handles all the module creation, hosting, patching and audio processing
class MainPatcher    : public Component,
                       public DragAndDropContainer,
                       public ChangeListener
{
public:
    MainPatcher();
    ~MainPatcher();

    void paint (Graphics&) override;
    void resized() override;
    
    /// Toggles between two patch-cord drawing routines
    void togglePatchCordType();
    
    /// Listener for key presses
    bool keyPressed (const KeyPress& key) override;
    
private:
    /// The class where each module's DSP routine gets implemented as nodes and patched together
    class AudioEngine : public AudioProcessorGraph
    {
    public:
        AudioEngine()
        {
            enableAllBuses();

            deviceManager.initialiseWithDefaultDevices (2, 2);
            deviceManager.addAudioCallback (&player);
            
            player.setProcessor (this);
            
            // Add an output node in the graph so that output modules may connect to it
            outputNode = addNode(std::make_unique<AudioGraphIOProcessor>(AudioGraphIOProcessor::audioOutputNode));
        }
        
        ~AudioEngine()
        {
            deviceManager.removeAudioCallback (&player);
        }
        
        /// Takes an array of Connections::IOid pairs (output, input) and iterates through it to apply the connections in the AudioProcessorGraph
        void applyAudioConnections(Array<std::pair<Connections::IOid, Connections::IOid>> connectionsToApply)
        {
            for (std::pair<Connections::IOid, Connections::IOid> connection : connectionsToApply)
            {
                NodeAndChannel source { {}, connection.first.second };
                NodeAndChannel destination { {}, connection.second.second };
                
                source.nodeID.uid = connection.first.first;
                destination.nodeID.uid = connection.second.first;
                
                addConnection ({ source, destination });
            }
            removeIllegalConnections();
        }
        
        /** Connects all the outlets of a node to the output node.
         This function should only be called on modules that are meant as an audio output to the patcher.
         Its use however, still allows for the outlets to be connected to other modules in the patcher, if they are made available */
        void connectToOuput(Node::Ptr nodeToConnect)
        {
            int connectionNumber = nodeToConnect->getProcessor()->getTotalNumOutputChannels();
            for (int i = 0; i < connectionNumber; i++)
            {
                addConnection ({ { nodeToConnect->nodeID, i }, { outputNode->nodeID, i } });
            }
        }
    private:
        AudioDeviceManager deviceManager;
        AudioProcessorPlayer player;
        Node::Ptr outputNode;
        
    } audioEngine;
    
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
    
    /** Registers all inlets and outlets of a module with the connections component.
     This function performs three jobs for each inlet/outlet:
     registers with connections, sets the resulting registry ID in the inlet/outlet and adds connections as a listener so that it may receive actions from it. */
    void registerInletsAndOutlets(Module*, uint32);
    
    /// Creates a module of type moduleClass at certain (top-left) point in the patcher, registers it to connections and adds it to audioEngine
    template <class> void createModule(Point<float>);
    
    /// Deletes a module and all its connections from the patcher and audioEngine, unregisters it from connections
    void deleteModule(ModuleBox*);
    /// Runs deleteModule() for every selected module
    void deleteAllSelectedModules();
    
    void mouseDown(const MouseEvent& e) override;
    void changeListenerCallback (ChangeBroadcaster* source) override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainPatcher)
};
