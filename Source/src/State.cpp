/*
  ==============================================================================

    State.cpp
    Created: 21 Aug 2025 11:13:29pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#include "State.h"
#include "modules/Modules.h"

State::State() : state("ui") {
    state.appendChild(juce::ValueTree{"modules"}, nullptr);
    state.appendChild(juce::ValueTree{"connections"}, nullptr);
    state.setProperty("showPortLabels", 0, nullptr);
    state.setProperty("patchCordType", 0, nullptr);
    
    state.addListener(this);
}
State::~State() {}

void copyValueTreeRecursively (juce::ValueTree& target, const juce::ValueTree& source)
{
    target.copyPropertiesFrom(source, nullptr);
    
    target.removeAllChildren(nullptr);
    
    for (int i = 0; i < source.getNumChildren(); ++i) {
        auto sourceChild = source.getChild(i);
        juce::ValueTree targetChild {sourceChild.getType()};
        
        // The only exception is a module tree that must contain a module type
        if (source.getType().toString() == "modules")
            targetChild.setProperty("type", sourceChild.getProperty("type"), nullptr);
        
        target.appendChild(targetChild, nullptr);
        
        copyValueTreeRecursively(targetChild, sourceChild);
    }
}

void State::save(juce::File file) {
    jassert(saveEngineState); // <- Callback must be registered with the engine!
    
    juce::FileOutputStream output (file);

    if (output.openedOk()) {
        output.setPosition (0);
        output.truncate();
        
        juce::ValueTree engineTree {"engine"};
        saveEngineState(engineTree);
        
        juce::ValueTree fileTree {"phi-state"};
        
        fileTree.appendChild(state, nullptr);
        fileTree.appendChild(engineTree, nullptr);
        
        fileTree.writeToStream(output);
        dirty = false;
        listeners.call([&] (auto& listener) { listener.fileSaved(file); });
    }
}

void State::load(juce::File file) {
    jassert(loadEngineState); // <- Callback must be registered with the engine!
    
    juce::FileInputStream input (file);

    if (input.openedOk()) {
        auto newTree = juce::ValueTree::readFromStream(input);
        
        copyValueTreeRecursively(state, newTree.getChildWithName("ui"));
        loadEngineState(newTree.getChildWithName("engine"));
        
        dirty = false;
        listeners.call([&] (auto& listener) { listener.fileLoaded(file); });
    }
}

void State::addModule(const std::string& type, int x, int y) {
    auto modulesTree = state.getChildWithName("modules");
    ModuleID moduleID (lastModuleID + 1);
    
    juce::ValueTree newModuleNode (moduleID.toString());
    newModuleNode.setProperty("type", juce::String(type), nullptr);

    modulesTree.appendChild(newModuleNode, nullptr);
    
    setModuleBounds(moduleID, {(int)x, (int)y, 0, 0});
}

void State::deleteModule(ModuleID moduleID)
{
    if (auto moduleNode = getModuleWithID(moduleID); moduleNode.isValid())
    {
        state.getChildWithName("modules").removeChild(moduleNode, nullptr);
        deleteAllModuleConnections(moduleID);
    }
}

void State::setModuleBounds(ModuleID moduleID, const juce::Rectangle<int>& bounds)
{
    if (auto moduleNode = getModuleWithID(moduleID); moduleNode.isValid())
        moduleNode.setProperty("bounds", bounds.toString(), nullptr);
}

void State::setModuleEnabled(ModuleID moduleID, bool isEnabled)
{
    if (auto moduleNode = getModuleWithID(moduleID); moduleNode.isValid())
        moduleNode.setProperty("enabled", isEnabled, nullptr);
}

void State::setModuleColour(ModuleID moduleID, const juce::Colour& colour)
{
    if (auto moduleNode = getModuleWithID(moduleID); moduleNode.isValid())
        moduleNode.setProperty("colour", colour.toString(), nullptr);
}

void State::deleteAllModuleConnections(ModuleID moduleID)
{
    auto connectionsTree = state.getChildWithName("connections");
    for (int j = connectionsTree.getNumChildren() - 1; j >= 0; --j) {
        auto connectionID = ConnectionID::fromString(connectionsTree.getChild(j).getType());
        
        if (connectionID.source.moduleID == moduleID || connectionID.destination.moduleID == moduleID)
            connectionsTree.removeChild(j, nullptr);
    }
}

void State::createConnection(ConnectionID connectionID)
{
    state.getChildWithName("connections").appendChild(juce::ValueTree(connectionID.toString()), nullptr);
}

void State::deleteConnection(ConnectionID connectionID)
{
    if (auto connectionNode = getConnectionWithID(connectionID); connectionNode.isValid())
        state.getChildWithName("connections").removeChild(connectionNode, nullptr);
}

void State::setConnectionColour(ConnectionID connectionID, const juce::Colour& colour)
{
    if (auto connectionNode = getConnectionWithID(connectionID); connectionNode.isValid())
        connectionNode.setProperty("colour", colour.toString(), nullptr);
}

void State::setShowPortLabels(ShowPortLabels show)
{
    state.setProperty("showPortLabels", (int)show, nullptr);
}

void State::setPatchCordType(PatchCordType type)
{
    state.setProperty("patchCordType", (int)type, nullptr);
}

juce::ValueTree State::getModuleWithID (ModuleID moduleID) {
    auto modulesTree = state.getChildWithName("modules");
    return modulesTree.getChildWithName(moduleID.toString());
}

juce::ValueTree State::getConnectionWithID (ConnectionID connectionID) {
    auto connectionsTree = state.getChildWithName("connections");
    return connectionsTree.getChildWithName(connectionID.toString());
}

void State::valueTreePropertyChanged (juce::ValueTree& tree, const juce::Identifier& property)
{
    dirty = true;
    
    auto key = property.toString();
    auto val = tree.getProperty(property);
    
    if (tree == state)
    {
        if (key == "showPortLabels")
        {
            auto show = (ShowPortLabels)(int)val;
            listeners.call([&] (auto& listener) { listener.showPortLabelsChanged(show); });
        }
        else if (key == "patchCordType")
        {
            auto type = (PatchCordType)(int)val;
            listeners.call([&] (auto& listener) { listener.patchCordTypeChanged(type); });
        }
    }
    else if (tree.getParent().getType().toString() == "modules")
    {
        auto moduleID = ModuleID::fromString(tree.getType());
       
        if (key == "bounds")
        {
            auto bounds = juce::Rectangle<int>::fromString(val.toString());
            listeners.call([&] (auto& listener) { listener.moduleBoundsChanged(moduleID, bounds); });
        }
        else if (key == "enabled")
        {
            bool isEnabled = val;
            listeners.call([&] (auto& listener) { listener.moduleEnabledChanged(moduleID, isEnabled); });
        }
        else if (key == "colour")
        {
            auto colour = juce::Colour::fromString(val.toString());
            listeners.call([&] (auto& listener) { listener.moduleColourChanged(moduleID, colour); });
        }
    }
    else if (tree.getParent().getType().toString() == "connections")
    {
        auto connectionID = ConnectionID::fromString(tree.getType());
        
        if (key == "colour")
        {
            auto colour = juce::Colour::fromString(val.toString());
            listeners.call([&] (auto& listener) { listener.connectionColourChanged(connectionID, colour); });
        }
    }
}

void State::valueTreeChildAdded (juce::ValueTree& parent, juce::ValueTree& tree)
{
    dirty = true;
    
    if (parent.getType().toString() == "modules" && tree.hasProperty("type"))
    {
        auto moduleID = ModuleID::fromString(tree.getType());
        
        if (moduleID > lastModuleID)
            lastModuleID = moduleID;
        
        auto processor = Modules::getInfoFromFromName(tree.getProperty("type"))->create();
        newModuleUICreated(processor->createUI(), moduleID);
        newProcessorCreated(std::move(processor), moduleID);
        
        listeners.call([&] (auto& listener) { listener.moduleAdded(moduleID); });
    }
    else if (parent.getType().toString() == "connections")
    {
        auto connectionID = ConnectionID::fromString(tree.getType());
        listeners.call([&] (auto& listener) { listener.connectionCreated(connectionID); });
    }
}

void State::valueTreeChildRemoved (juce::ValueTree& parent, juce::ValueTree& tree, int index)
{
    dirty = true;
    
    if (parent == state && tree.getType().toString() == "modules")
    {
        listeners.call([&] (auto& listener) { listener.allModulesDeleted(); });
    }
    else if (parent.getType().toString() == "modules")
    {
        auto moduleID = ModuleID::fromString(tree.getType());
        listeners.call([&] (auto& listener) { listener.moduleDeleted(moduleID); });
    }
    else if (parent.getType().toString() == "connections")
    {
        auto connectionID = ConnectionID::fromString(tree.getType());
        listeners.call([&] (auto& listener) { listener.connectionDeleted(connectionID); });
    }
}
