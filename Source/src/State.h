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

struct ModuleID {
    juce::uint32 value;
    
    ModuleID() : value(0) {}
    ModuleID(int init) : value(init) {};
    ModuleID(juce::uint32 init) : value(init) {};
    
    ModuleID& operator++() { value++; return *this; }
    ModuleID operator++(int) {
        ModuleID temp = *this;
        value++;
        return temp;
    }
    
    operator juce::uint32() const { return value; }
    
    operator juce::AudioProcessorGraph::NodeID() const {
        return  juce::AudioProcessorGraph::NodeID(value);
    }
    
    static ModuleID fromString(const juce::String& s) {
        return s.getIntValue();
    }
    
    static ModuleID fromString(const juce::Identifier& identifier) {
        return fromString(identifier.toString());
    }
    
    juce::String toString() const {
        return juce::String(value);
    }
};

namespace std {
    template <>
    struct hash<ModuleID> {
        std::size_t operator()(const ModuleID& m) const {
            return m.value;
        }
    };
}

struct PortID {
    int value;
    
    PortID() : value(0) {};
    PortID(int init) : value(init) {};
    operator int() const { return value; }
    
    static PortID fromString(const juce::String& s) {
        return s.getIntValue();
    }
    
    juce::String toString() const {
        return juce::String(value);
    }
};

struct ModulePortID
{
    ModuleID moduleID;
    PortID portID;
    
    ModulePortID() = default;
    ModulePortID(ModuleID moduleID, PortID portID) : moduleID(moduleID), portID(portID) {}
    ModulePortID(const ModulePortID& other) : moduleID(other.moduleID), portID(other.portID) {}
    ModulePortID(ModulePortID&& other) noexcept : moduleID(other.moduleID), portID(other.portID) {}
    
    operator juce::AudioProcessorGraph::NodeAndChannel() const {
        return { moduleID, portID };
    }
    
    void operator=(const ModulePortID& other) {moduleID = other.moduleID; portID = other.portID;}
    
    bool operator== (const ModulePortID& other) const {
        return moduleID == other.moduleID && portID == other.portID;
    }
    
    size_t hash() const { return (size_t)moduleID ^ ((size_t)portID << 1); }
    
    static ModulePortID fromString(const juce::String& string) {
        return {
            ModuleID::fromString(string.upToFirstOccurrenceOf(":", false, false)),
            PortID::fromString(string.fromLastOccurrenceOf(":", false, false))
        };
    }
    
    juce::String toString() const {
        return juce::String(moduleID) + ":" + juce::String(portID);
    }
};

struct ConnectionID
{
    ModulePortID source;
    ModulePortID destination;
    
    ConnectionID() = default;
    ConnectionID(const ConnectionID& other) : source(other.source), destination(other.destination) {}
    ConnectionID(ModulePortID source, ModulePortID destination) : source(source), destination(destination) {}
    
    void operator=(const ConnectionID& other) {source = other.source; destination = other.destination;}
    
    bool operator==(const ConnectionID& other) const {
        return source == other.source && destination == other.destination;
    }
    
    operator juce::AudioProcessorGraph::Connection() const {
        return {source, destination};
    }
    
    static ConnectionID fromString(const juce::String& string) {
        return {
            ModulePortID::fromString(string.upToFirstOccurrenceOf("-", false, false)),
            ModulePortID::fromString(string.fromLastOccurrenceOf("-", false, false))
        };
    }
    
    static ConnectionID fromString(const juce::Identifier& identifier) {
        return fromString(identifier.toString());
    }
    
    juce::String toString() const {
        return source.toString() + "-" + destination.toString();
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

enum class PortType { Inlet, Outlet };

enum class ShowPortLabels { Off, On };

enum class PatchCordType { S, Arc };

struct State : juce::ValueTree::Listener {
    State();
    ~State();
    // ========================================================================
    
    /// Callback to receive the created module UI
    std::function<void(std::unique_ptr<ModuleUI>, ModuleID)> newModuleUICreated;
    /// Callback to receive the created module Processor
    std::function<void(std::unique_ptr<ModuleProcessor>, ModuleID)> newProcessorCreated;

    /// Sets the first ID to use for modules
    void setFirstModuleID(ModuleID moduleID) { lastModuleID = moduleID; }
    
    void save(juce::File);
    void load(juce::File);
    
    // State Setters
    void addModule(const std::string& type, int x, int y);
    void deleteModule(ModuleID moduleID);
    
    void setModuleEnabled(ModuleID, bool isEnabled);
    void setModuleBounds(ModuleID, const juce::Rectangle<int>& bounds);
    void setModuleColour(ModuleID, const juce::Colour& colour);
    
    void createConnection(ConnectionID);
    void deleteConnection(ConnectionID);
    void setConnectionColour(ConnectionID, const juce::Colour& colour);
    
    void setShowPortLabels(ShowPortLabels);
    void setPatchCordType(PatchCordType);
    
    // ========================================================================
    
    struct Listener {
        virtual ~Listener() = default;
        
        virtual void moduleAdded(ModuleID) {};
        virtual void moduleDeleted(ModuleID) {};
        virtual void allModulesDeleted() {};
        
        virtual void moduleEnabledChanged(ModuleID, bool isEnabled) {};
        virtual void moduleBoundsChanged(ModuleID, const juce::Rectangle<int>& bounds) {};
        virtual void moduleColourChanged(ModuleID, const juce::Colour& colour) {};
        
        virtual void connectionCreated(ConnectionID) {};
        virtual void connectionDeleted(ConnectionID) {};
        virtual void connectionColourChanged(ConnectionID, const juce::Colour& colour) {};
        
        virtual void showPortLabelsChanged(ShowPortLabels) {};
        virtual void patchCordTypeChanged(PatchCordType) {};
        
        virtual void fileLoaded(juce::File) {}
        virtual void fileSaved(juce::File) {}
    };
    
    void addListener (Listener* listener) { listeners.add(listener); }
    void removeListener (Listener* listener) { listeners.remove(listener); }

    // ========================================================================
private:
    juce::ValueTree state;
    juce::ListenerList<Listener> listeners;
    
    ModuleID lastModuleID {0};
    
    void deleteAllModuleConnections(ModuleID);
    
    juce::ValueTree getModuleWithID (ModuleID);
    juce::ValueTree getConnectionWithID (ConnectionID);
    
    void valueTreePropertyChanged (juce::ValueTree&, const juce::Identifier&) override;
    void valueTreeChildAdded (juce::ValueTree&, juce::ValueTree&) override;
    void valueTreeChildRemoved (juce::ValueTree&, juce::ValueTree&, int) override;
};
