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

//==============================================================================
/// The main view of the Phi, this class handles all the module creation, hosting, patching and audio processing
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
    /// The processor where each module's DSP routine gets implemented as nodes and patched together
    class AudioEngine : public AudioProcessorGraph
    {
    public:
        AudioEngine()
        {
            enableAllBuses();

            deviceManager.initialiseWithDefaultDevices (2, 2);
            deviceManager.addAudioCallback (&player);
            
            player.setProcessor (this);
            
            // Add an output node in the graph
            // so that output modules may connect to it
            outputNode = addNode(std::make_unique<AudioGraphIOProcessor>(AudioGraphIOProcessor::audioOutputNode));
        }
        
        ~AudioEngine()
        {
            deviceManager.removeAudioCallback (&player);
        }
        
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
        
        void connectToOuput(Node::Ptr nodeToConnect, int connectionNumber)
        {
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
    
    void mouseDown(const MouseEvent& e) override;
    
    void changeListenerCallback (ChangeBroadcaster* source) override;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainPatcher)
};
