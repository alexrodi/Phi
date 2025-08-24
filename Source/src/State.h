/*
  ==============================================================================

    State.h
    Created: 21 Aug 2025 11:13:14pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ui/ModuleUI.h"
#include "dsp/ModuleProcessor.h"

using ModuleID = juce::uint32;
using PortID = int;

struct ModulePortID
{
    ModuleID moduleID;
    PortID portID;
    
    ModulePortID() : moduleID(0), portID(0) {}
    ModulePortID(ModuleID moduleID, PortID portID) : moduleID(moduleID), portID(portID) {}
    ModulePortID(const ModulePortID& other) : moduleID(other.moduleID), portID(other.portID) {}
    ModulePortID(ModulePortID&& other) noexcept : moduleID(other.moduleID), portID(other.portID) {}
    
    operator juce::AudioProcessorGraph::NodeAndChannel() const {
        return { juce::AudioProcessorGraph::NodeID{moduleID}, portID };
    }
    
    void operator=(const ModulePortID& other) {moduleID = other.moduleID; portID = other.portID;}
    
    bool operator== (const ModulePortID& other) const {
        return moduleID == other.moduleID && portID == other.portID;
    }
    
    size_t hash() const { return (size_t)moduleID ^ ((size_t)portID << 1); }
};

struct ConnectionID
{
    ModulePortID source;
    ModulePortID destination;
    
    ConnectionID() {}
    ConnectionID(const ConnectionID& other) : source(other.source), destination(other.destination) {}
    ConnectionID(ModulePortID source, ModulePortID destination) : source(source), destination(destination) {}
    
    operator juce::AudioProcessorGraph::Connection() const {
        return {source, destination};
    }
    
    void operator=(const ConnectionID& other) {source = other.source; destination = other.destination;}
    
    bool operator==(const ConnectionID& other) const {
        return source == other.source && destination == other.destination;
    }
};

namespace std {
    template <>
    struct hash<ConnectionID> {
        std::size_t operator()(const ConnectionID& c) const {
            return c.source.hash() ^ (c.destination.hash() << 1);
        }
    };
}

struct ModuleInfo {
    ModuleInfo(juce::String name) : name(name) {}
    juce::String name;
    virtual std::unique_ptr<ModuleProcessor> create() = 0;
    virtual ~ModuleInfo() = default;
};

enum class PortType { Inlet, Outlet };

enum class ShowPortLabels { Off, On };

enum class PatchCordType { S, Arc };

struct State {
    State();
    ~State();
    // ========================================================================
    
    /// Callback to receive the created module UI
    std::function<void(std::unique_ptr<ModuleUI>, ModuleID)> newModuleUICreated;
    /// Callback to receive the created module Processor
    std::function<void(std::unique_ptr<ModuleProcessor>, ModuleID)> newProcessorCreated;

    /// Sets the first ID to use for modules
    void setFirstModuleID(ModuleID moduleID) { lastID = moduleID; }
    
    // State Setters
    void addModule(std::unique_ptr<ModuleInfo> moduleInfo, int x, int y);
    void deleteModule(ModuleID moduleID);
    
    void setModuleEnabled(ModuleID moduleID, bool isEnabled);
    void setModuleBounds(ModuleID moduleID, const juce::Rectangle<int>& bounds);
    
    void createConnection(ConnectionID connectionID);
    void deleteConnection(ConnectionID connectionID);
    
    void setShowPortLabels(ShowPortLabels show);
    void setPatchCordType(PatchCordType type);
    
    // ========================================================================
    
    struct Listener {
        virtual ~Listener() = default;
        
        virtual void moduleAdded(ModuleID) {};
        virtual void moduleDeleted(ModuleID) {};
        
        virtual void moduleEnabledChanged(ModuleID, bool isEnabled) {};
        virtual void moduleBoundsChanged(ModuleID, juce::Rectangle<int> bounds) {};
        
        virtual void connectionCreated(ConnectionID) {};
        virtual void connectionDeleted(ConnectionID) {};
        
        virtual void showPortLabelsChanged(ShowPortLabels) {};
        virtual void patchCordTypeChanged(PatchCordType) {};
    };
    
    void addListener (Listener* listener) { listeners.add(listener); }
    void removeListener (Listener* listener) { listeners.remove(listener); }

    // ========================================================================
private:
    juce::ValueTree state;
    juce::ListenerList<Listener> listeners;
    
    ModuleID lastID;
    
    void deleteAllConnectionsToFromModule(ModuleID);
    ConnectionID getConnectionID (juce::ValueTree);
    juce::ValueTree newConnectionNode (ConnectionID);
};
