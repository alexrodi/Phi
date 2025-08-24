/*
  ==============================================================================

    Connections.h
    Created: 20 Feb 2020 8:18:35pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

#include "../State.h"
#include "PhiColours.h"

struct Patcher;

//==============================================================================

/// The connection manager and drawer
class Connections : public juce::Component,
                    public State::Listener,
                    public juce::ChangeListener,
                    public juce::LassoSource<ConnectionID>
{
public:
    Connections(State& state, const Patcher& patcher);
    ~Connections();
    
    void paint (juce::Graphics&) override;
    void resized () override;
    
    void deleteAllSelected();
    
private:
    struct Connection {
        juce::Path path;
        juce::Colour colour;
    };
    
    struct HeldConnection : Connection {
        juce::Point<float> anchor;
    };
    
    //==============================================================================
    
    State& state;
    const Patcher& patcher;
    
    std::unordered_map<ConnectionID, Connection> connections;
    juce::LassoComponent<ConnectionID> lasso;
    juce::SelectedItemSet<ConnectionID> selectedConnections;
    
    std::unique_ptr<HeldConnection> heldConnection;
    
    static constexpr float CORD_WEIGHT = 0.2f;
    juce::PathStrokeType selectedStroke {2.0f};
    juce::PathStrokeType patchCordStroke = {5.0f, juce::PathStrokeType::JointStyle::mitered, juce::PathStrokeType::EndCapStyle::rounded};
    
    struct MouseListener : juce::MouseListener {
        MouseListener(Connections* owner) : owner(owner) {}
        
        void mouseDown(const juce::MouseEvent& e) override { owner->onMouseDown(e); };
        void mouseUp(const juce::MouseEvent& e) override { owner->onMouseUp(e); };
        void mouseDrag(const juce::MouseEvent& e) override { owner->onMouseDrag(e); };
        
    private:
        Connections* owner;
    } mouseListener;
    
    ConnectionID hitConnectionID;
    
    PatchCordType patchCordType = PatchCordType::S;
    ShowPortLabels showPortLabels = ShowPortLabels::Off;
    
    //==============================================================================
    
    /// A callback for drawing patch cords that applies a vertical weight
    static juce::Path getArcPatchCordPath (const juce::Line<float>&);
    /// A callback for drawing patch cords with a horizontal S shape
    static juce::Path getSPatchCordPath (const juce::Line<float>&);
    
    void updateConnectionPath(ConnectionID);
    void updateHeldConnectionPath(const juce::MouseEvent& e);
    void tryCreateHeldConnection(const juce::MouseEvent &e);
    
    /// Runs a function on every currently selected Connection
    template<class CallbackType>
    void forEachSelected(CallbackType);
    
    void onMouseDown(const juce::MouseEvent& e);
    void onMouseUp(const juce::MouseEvent& e);
    void onMouseDrag(const juce::MouseEvent& e);
    
    // State listener overrides
    void connectionCreated(ConnectionID) override;
    void connectionDeleted(ConnectionID) override;
    void moduleBoundsChanged(ModuleID, juce::Rectangle<int>) override;
    void patchCordTypeChanged(PatchCordType) override;
    void showPortLabelsChanged(ShowPortLabels show) override;
    
    void findLassoItemsInArea (juce::Array<ConnectionID>& itemsFound, const juce::Rectangle<int>& area) override;
    juce::SelectedItemSet<ConnectionID>& getLassoSelection() override;
    
    bool hitTest(int x, int y) override;
    
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;
    
    void parentHierarchyChanged() override;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Connections)
};
