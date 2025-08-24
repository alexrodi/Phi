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
    /// A constant output node to Port output modules to
    Node::Ptr outputNode;
    
    void moduleDeleted(ModuleID moduleID) override;
    
    // Assure flush-to-zero
    void processBlock (juce::AudioBuffer<float>&  audio, juce::MidiBuffer& midi) override {
        juce::ScopedNoDenormals nodenormals;
        juce::AudioProcessorGraph::processBlock (audio, midi);
    }
    
};
