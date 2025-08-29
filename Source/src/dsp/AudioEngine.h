/*
  ==============================================================================

    AudioEngine.h
    Created: 25 Apr 2020 10:58:41pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

#include "../State.h"

/// The class where each module's DSP routine gets implemented as nodes and patched together
struct AudioEngine : juce::AudioProcessorGraph,
                     State::Listener
{
    AudioEngine(State& state);
    ~AudioEngine();

private:
    State& state;
    
    /// Interfaces with output devices
    juce::AudioDeviceManager deviceManager;
    
    /// Allows to playback our Processor Graph
    juce::AudioProcessorPlayer player;
    
    /// A constant output node to plug output modules into
    Node::Ptr mainOutput;
    
    /** Connects all the outlets of a node to the output node.
        This function should only be called on modules that are meant as an audio output to the patcher.
        Its use however, still allows for the outlets to be connected to other modules in the patcher, if they are made available */
    void connectToOuput(Node::Ptr);
    
    void resetEngine();
    
    void moduleDeleted(ModuleID) override;
    void connectionCreated(ConnectionID) override;
    void connectionDeleted(ConnectionID) override;
    void moduleEnabledChanged(ModuleID, bool) override;
    void allModulesDeleted() override;
    
    // Assure flush-to-zero
    void processBlock (juce::AudioBuffer<float>&  audio, juce::MidiBuffer& midi) override {
        juce::ScopedNoDenormals nodenormals;
        juce::AudioProcessorGraph::processBlock (audio, midi);
    }
};
