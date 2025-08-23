/*
  ==============================================================================

    MainPatcher.h
    Created: 29 Mar 2020 12:51:46am
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

#include "../State.h"
#include "ModuleBox.h"
#include "../Modules.h"
#include "Connections.h"

//==============================================================================
/// The main view of Phi, this class handles all the module UIs and holds the connection manager
struct Patcher : juce::Component,
                 State::Listener,
                 juce::ComponentDragger,
                 juce::LassoSource<ModuleID>,
                 juce::ChangeListener
{
    explicit Patcher(State&);
    ~Patcher();
    
    const PortUI* getPortUI (ModulePortID, PortType) const;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    void mouseDown(const juce::MouseEvent& e) override;
    void mouseUp(const juce::MouseEvent& e) override;
    void mouseDrag(const juce::MouseEvent& e) override;
    void mouseMove(const juce::MouseEvent& e) override;
    
    bool keyPressed (const juce::KeyPress& key) override;
    
    
private:
    /// A reference to the state
    State& state;
    
    /// The connection manager
    Connections connections;
    
    /// All module boxes that belong to this patcher
    std::unordered_map<ModuleID, std::unique_ptr<ModuleBox>> modules;
    
    /// A set of selected modules
    juce::SelectedItemSet<ModuleID> selectedModuleIDs;
    
    juce::LassoComponent<ModuleID> lasso;
    
    juce::TooltipWindow tooltip;
    
    bool selectionResult = false;
    
    std::optional<ModuleID> getBoxModuleID(const ModuleBox&);
    std::optional<ModulePortID> getPortID(const PortUI&);
    
    void openMenu(const juce::MouseEvent& e);
    
    void deleteModule(ModuleID nodeID);
    
    /// Opens a ColourSelector for the selected modules' highlight colour
    void openColourSelector();
    
    /// Runs a funtion on every currently selected ModuleBox
    template<class CallbackType>
    void forEachSelected(CallbackType);
    
    // State listener overrides
    void moduleBoundsChanged(ModuleID moduleID, juce::Rectangle<int> bounds) override;
    void moduleDeleted(ModuleID moduleID) override;
    
    void findLassoItemsInArea (juce::Array<ModuleID>& itemsFound, const juce::Rectangle<int>& area) override;
    juce::SelectedItemSet<ModuleID>& getLassoSelection() override;
    
    void changeListenerCallback(juce::ChangeBroadcaster*) override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Patcher)
};
