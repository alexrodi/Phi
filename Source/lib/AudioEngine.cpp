/*
  ==============================================================================

    AudioEngine.cpp
    Created: 27 Apr 2020 10:58:41pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#include "AudioEngine.h"

AudioEngine::AudioEngine()
{
    enableAllBuses();

    deviceManager.initialiseWithDefaultDevices (2, 2);
    deviceManager.addAudioCallback (&player);
    
    player.setProcessor (this);
    
    // Add an output node in the graph so that output modules may connect to it
    outputNode = addNode(std::make_unique<AudioGraphIOProcessor>(AudioGraphIOProcessor::audioOutputNode));
}


AudioEngine::~AudioEngine()
{
    deviceManager.removeAudioCallback (&player);
}


void AudioEngine::applyAudioConnections(Array<std::pair<Connections::PlugID, Connections::PlugID>> connectionsToApply)
{
    for (std::pair<Connections::PlugID, Connections::PlugID> connection : connectionsToApply)
    {
        NodeAndChannel source { {}, connection.first.second };
        NodeAndChannel destination { {}, connection.second.second };
        
        source.nodeID.uid = connection.first.first;
        destination.nodeID.uid = connection.second.first;
        
        addConnection ({ source, destination });
    }
    removeIllegalConnections();
}


void AudioEngine::connectToOuput(Node::Ptr nodeToConnect)
{
    int connectionNumber = nodeToConnect->getProcessor()->getTotalNumOutputChannels();
    for (int i = 0; i < connectionNumber; i++)
    {
        addConnection ({ { nodeToConnect->nodeID, i }, { outputNode->nodeID, i } });
    }
}
