/*
  ==============================================================================

    State.cpp
    Created: 21 Aug 2025 11:13:29pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#include "State.h"

State::State() : state("PhiState") {
    state.addChild(juce::ValueTree{"modules"}, -1, nullptr);
    state.addChild(juce::ValueTree{"connections"}, -1, nullptr);
}
State::~State() {}

void State::save(juce::File file) {
    state.writeToStream(*file.createOutputStream());
}

void State::load(juce::File file) {
    state.readFromStream(*file.createInputStream());
}

void State::addModule(std::unique_ptr<ModuleInfo> moduleInfo, int x, int y) {
    auto modulesTree = state.getChildWithName("modules");
    auto moduleID = lastID++;
    
    juce::ValueTree newModuleNode("module");
    newModuleNode.setProperty("type", moduleInfo->name, nullptr);
    newModuleNode.setProperty("id", (int)moduleID, nullptr);

    modulesTree.addChild(newModuleNode, -1, nullptr);
    
    auto processor = moduleInfo->create();
    newModuleUICreated(processor->createUI(), moduleID);
    newProcessorCreated(std::move(processor), moduleID);
    
    listeners.call([&] (auto& listener) { listener.moduleAdded(moduleID); });
    
    setModuleBounds(moduleID, {(int)x, (int)y, 0, 0});
}

void State::deleteModule(ModuleID moduleID)
{
    if (auto moduleNode = getModuleWithID(moduleID); moduleNode.isValid())
    {
        state.getChildWithName("modules").removeChild(moduleNode, nullptr);
        deleteAllModuleConnections(moduleID);
        
        listeners.call([&] (auto& listener) { listener.moduleDeleted(moduleID); });
    }
}

void State::setModuleBounds(ModuleID moduleID, const juce::Rectangle<int>& bounds)
{
    if (auto moduleNode = getModuleWithID(moduleID); moduleNode.isValid())
    {
        moduleNode.setProperty("bounds", bounds.toString(), nullptr);
        listeners.call([&] (auto& listener) { listener.moduleBoundsChanged(moduleID, bounds); });
    }
}

void State::setModuleEnabled(ModuleID moduleID, bool isEnabled)
{
    if (auto moduleNode = getModuleWithID(moduleID); moduleNode.isValid())
    {
        moduleNode.setProperty("enabled", isEnabled, nullptr);
        listeners.call([&] (auto& listener) { listener.moduleEnabledChanged(moduleID, isEnabled); });
    }
}

void State::setModuleColour(ModuleID moduleID, const juce::Colour& colour)
{
    if (auto moduleNode = getModuleWithID(moduleID); moduleNode.isValid())
    {
        moduleNode.setProperty("colour", colour.toString(), nullptr);
        listeners.call([&] (auto& listener) { listener.moduleColourChanged(moduleID, colour); });
    }
}

void State::deleteAllModuleConnections(ModuleID moduleID)
{
    auto connectionsTree = state.getChildWithName("connections");
    if (!connectionsTree.isValid()) return;
    
    for (int j = connectionsTree.getNumChildren() - 1; j >= 0; --j)
    {
        auto connectionID = getConnectionID(connectionsTree.getChild(j));
        
        if (connectionID.source.moduleID == moduleID || connectionID.destination.moduleID == moduleID)
        {
            connectionsTree.removeChild(j, nullptr);
            listeners.call([&] (auto& listener) { listener.connectionDeleted(connectionID); });
        }
    }
}

void State::createConnection(ConnectionID connectionID)
{
    state.getChildWithName("connections").addChild(newConnectionNode(connectionID), -1, nullptr);
    listeners.call([&] (auto& listener) { listener.connectionCreated(connectionID); });
}

void State::deleteConnection(ConnectionID connectionID)
{
    if (auto connectionNode = getConnectionWithID(connectionID); connectionNode.isValid())
    {
        state.getChildWithName("connections").removeChild(connectionNode, nullptr);
        listeners.call([&] (auto& listener) { listener.connectionDeleted(connectionID); });
    }
}

void State::setConnectionColour(ConnectionID connectionID, const juce::Colour& colour)
{
    if (auto connectionNode = getConnectionWithID(connectionID); connectionNode.isValid())
    {
        connectionNode.setProperty("colour", colour.toString(), nullptr);
        listeners.call([&] (auto& listener) { listener.connectionColourChanged(connectionID, colour); });
    }
}

void State::setShowPortLabels(ShowPortLabels show)
{
    state.setProperty("showPortLabels", (int)show, nullptr);
    listeners.call([&] (auto& listener) { listener.showPortLabelsChanged(show); });
}

void State::setPatchCordType(PatchCordType type)
{
    state.setProperty("patchCordType", (int)type, nullptr);
    listeners.call([&] (auto& listener) { listener.patchCordTypeChanged(type); });
}


ConnectionID State::getConnectionID (juce::ValueTree node) {
    return {
        {
            (ModuleID)(int)node.getProperty("sourceModuleID"),
            (int)node.getProperty("sourcePortID")
        },
        {
            (ModuleID)(int)node.getProperty("destModuleID"),
            (int)node.getProperty("destPortID")
        }
    };
}

juce::ValueTree State::newConnectionNode (ConnectionID connectionID) {
    juce::ValueTree newConnectionNode("connection");
    
    newConnectionNode.setProperty("sourceModuleID", (int)connectionID.source.moduleID, nullptr);
    newConnectionNode.setProperty("sourcePortID", connectionID.source.portID, nullptr);
    newConnectionNode.setProperty("destModuleID", (int)connectionID.destination.moduleID, nullptr);
    newConnectionNode.setProperty("destPortID", connectionID.destination.portID, nullptr);
    
    return newConnectionNode;
}

juce::ValueTree State::getModuleWithID (ModuleID moduleID) {
    auto modulesTree = state.getChildWithName("modules");
    return modulesTree.getChildWithProperty("id", (int)moduleID);
}

juce::ValueTree State::getConnectionWithID (ConnectionID connectionID) {
    auto connectionsTree = state.getChildWithName("connections");
    
    for (int i = 0; i < connectionsTree.getNumChildren(); i++) {
        auto child = connectionsTree.getChild(i);
        
        if (getConnectionID(child) == connectionID)
            return child;
    }
    
    return {};
}
