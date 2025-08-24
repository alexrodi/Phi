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
    // Initialise the device manager and add the player
    deviceManager.initialise(2, 2, nullptr, true, juce::String(), nullptr);
    deviceManager.addAudioCallback(&player);
    
    // Add the main output node to the graph
    player.setProcessor(&*this);
    mainOutput = addNode(std::make_unique<AudioGraphIOProcessor>(AudioGraphIOProcessor::audioOutputNode));
    
    state.setFirstModuleID(mainOutput->nodeID.uid + 1);
    state.newProcessorCreated = [&] (auto processor, auto moduleID) {
        bool isOutput = processor->isOutput;
        if (auto node = addNode(std::move(processor), std::make_optional<NodeID>(moduleID))) {
            // When we detect an output module, we hook it up to the main output node
            if (isOutput)
                connectToOuput(node);
        } else {
            state.deleteModule(moduleID);
        }
    };
    
    state.addListener(this);
}

AudioEngine::~AudioEngine()
{
    deviceManager.removeAudioCallback(&player);
    player.setProcessor(nullptr);
    state.removeListener(this);
}

void AudioEngine::moduleDeleted(ModuleID moduleID) {
    removeNode((NodeID)moduleID);
}

void AudioEngine::connectionCreated(ConnectionID connectionID) {
    if (!addConnection(connectionID))
        state.deleteConnection(connectionID);
}

void AudioEngine::connectionDeleted(ConnectionID connectionID) {
    removeConnection(connectionID);
}

void AudioEngine::moduleEnabledChanged(ModuleID moduleID, bool isEnabled) {
    getNodeForId((NodeID)moduleID)->getProcessor()->suspendProcessing(!isEnabled);
}

void AudioEngine::connectToOuput(Node::Ptr nodeToConnect)
{
    int connectionNumber = nodeToConnect->getProcessor()->getTotalNumOutputChannels();
    
    for (int i = 0; i < connectionNumber; i++)
        addConnection ({ {nodeToConnect->nodeID, i}, {mainOutput->nodeID, i} });
}
