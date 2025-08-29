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
    player.setProcessor(this);
    
    resetEngine();
    
    state.newProcessorCreated = [&] (std::unique_ptr<ModuleProcessor> processor, auto moduleID) {
        bool isOutput = processor->isOutput;
        if (auto node = addNode(std::move(processor), std::make_optional<NodeID>(moduleID))) {
            // When we detect an output module, we hook it up to the main output node
            if (isOutput)
                connectToOuput(node);
        } else {
            state.deleteModule(moduleID);
        }
    };
    
    state.saveEngineState = [&] (auto tree) {
        for (auto& node : getNodes()) {
            if (auto* processor = dynamic_cast<ModuleProcessor*>(node->getProcessor())) {
                auto child = processor->params.copyState();
                
                child.setProperty("id", (int)node->nodeID.uid, nullptr);
                
                tree.appendChild(child, nullptr);
            }
        }
    };
    
    state.loadEngineState = [&] (auto tree) {
        for (int i = 0; i < tree.getNumChildren(); ++i) {
            auto child = tree.getChild(i);
            if (auto* node = getNodeForId((NodeID)(int)child.getProperty("id"))) {
                if (auto* processor = dynamic_cast<ModuleProcessor*>(node->getProcessor())) {
                    child.removeProperty("id", nullptr);
                    processor->params.state.copyPropertiesAndChildrenFrom(child, nullptr);
                }
            }
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

void AudioEngine::allModulesDeleted() {
    resetEngine();
}

void AudioEngine::resetEngine() {
    clear();
    
    // Add the main output node to the graph
    mainOutput = addNode(
        std::make_unique<AudioGraphIOProcessor>(AudioGraphIOProcessor::audioOutputNode),
        std::make_optional<NodeID>(1)
    );
}
