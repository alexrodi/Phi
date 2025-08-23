/*
  ==============================================================================

    AudioEngine.cpp
    Created: 27 Apr 2020 10:58:41pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#include "AudioEngine.h"

AudioEngine::AudioEngine(State& state) : state(state)
{
    state.newProcessorCreated = [&] (auto processor, auto moduleID) {
        addNode(std::move(processor), std::make_optional<NodeID>(moduleID));
    };
    
    // Initialise the device manager and add the player
    deviceManager.initialise(2, 2, nullptr, true, juce::String(), nullptr);
    deviceManager.addAudioCallback(&player);
    
    // Add the output node to the graph
    player.setProcessor(&*this);
    outputNode = addNode(std::make_unique<AudioGraphIOProcessor>(AudioGraphIOProcessor::audioOutputNode));
    
    state.setFirstModuleID(outputNode->nodeID.uid + 1);
}

AudioEngine::~AudioEngine()
{
    deviceManager.removeAudioCallback(&player);
    player.setProcessor(nullptr);
}

void AudioEngine::moduleDeleted(ModuleID moduleID) {
    juce::AudioProcessorGraph::removeNode((NodeID)moduleID);
}

