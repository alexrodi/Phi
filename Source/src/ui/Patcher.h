/*
  ==============================================================================

    Patcher.h
    Created: 29 Mar 2020 12:51:46am
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

#include "../State.h"
#include "ModuleBox.h"
#include "Connections.h"
#include "../modules/Modules.h"

//==============================================================================
/// The main view of Phi, this class handles all the module UIs and holds the connection manager
struct Patcher : juce::Component,
                 State::Listener,
                 juce::LassoSource<ModuleID>,
                 juce::ChangeListener
{
    explicit Patcher(State&);
    ~Patcher();
    
    const PortUI* getPortUI (ModulePortID, PortType) const;
    
    std::optional<ModuleID> getModuleID(const ModuleBox&) const;
    std::optional<ModulePortID> getModulePortID(const PortUI&) const;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    bool keyPressed (const juce::KeyPress& key) override;
    
private:
    //==============================================================================
    /// A reference to the state
    State& state;
    
    /// The connection manager
    Connections connections;
    
    /// All module boxes that belong to this patcher
    std::unordered_map<ModuleID, std::unique_ptr<ModuleBox>> modules;
    
    /// A set of selected modules
    juce::SelectedItemSet<ModuleID> selectedModuleIDs;
    
    juce::LassoComponent<ModuleID> lasso;
    
    HoverPopupWindow hoverPopup;
    
    struct MouseListener : juce::MouseListener {
        MouseListener(Patcher* owner) : owner(owner) {}
        
        void mouseDown(const juce::MouseEvent& e) override { owner->onMouseDown(e); };
        void mouseUp(const juce::MouseEvent& e) override { owner->onMouseUp(e); };
        void mouseDrag(const juce::MouseEvent& e) override { owner->onMouseDrag(e); };
        
    private:
        Patcher* owner;
    } mouseListener;
    
    struct ModuleDragger {
        void addOnMouseDown(const std::vector<ModuleBox*>& modules) {
            modulePositions.clear();
            
            for (auto* box : modules)
                modulePositions[box] = box->getPosition();
        }
        
        void moveOnMouseDrag(const juce::MouseEvent& e) {
            for (auto& [box, position] : modulePositions)
                box->setBounds(box->getBounds().withPosition(position + e.getOffsetFromDragStart()));
        }
        
    private:
        std::map<ModuleBox*, juce::Point<int>> modulePositions;
    } moduleDragger;
    
    std::vector<ModuleBox*> getSelectedModules() {
        std::vector<ModuleBox*> modules;
        forEachSelected([&] (auto moduleID, auto& moduleBox) { modules.push_back(&moduleBox); });
        return modules;
    }
    
    //==============================================================================
    ShowPortLabels showPortLabels = ShowPortLabels::Off;
    
    void openMenu(const juce::MouseEvent& e);
    
    void deleteModule(ModuleID nodeID);
    
    /// Runs a funtion on every currently selected ModuleBox
    template<class CallbackType>
    void forEachSelected(CallbackType);
    
    void onMouseDown(const juce::MouseEvent& e);
    void onMouseUp(const juce::MouseEvent& e);
    void onMouseDrag(const juce::MouseEvent& e);
    
    juce::Rectangle<int> getContentBounds();
    
    // State listener overrides
    void moduleBoundsChanged(ModuleID moduleID, const juce::Rectangle<int>& bounds) override;
    void showPortLabelsChanged(ShowPortLabels) override;
    void moduleDeleted(ModuleID moduleID) override;
    
    void findLassoItemsInArea (juce::Array<ModuleID>& itemsFound, const juce::Rectangle<int>& area) override;
    juce::SelectedItemSet<ModuleID>& getLassoSelection() override;
    
    void changeListenerCallback(juce::ChangeBroadcaster*) override;
    
    void parentHierarchyChanged() override;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Patcher)
};
