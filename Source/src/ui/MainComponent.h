
#pragma once

#include "../State.h"
#include "../FileManager.h"
#include "Patcher.h"
#include "PhiColours.h"
#include "component/PhiSliderButton.h"

//==============================================================================
/// The top-most component that includes all the content
struct MainComponent : juce::Component,
                       State::Listener

{
//==============================================================================
    explicit MainComponent(State&, FileManager&);
    ~MainComponent();

    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    PhiLookAndFeel phiLookAndFeel;
    
    //==============================================================================
    State& state;
    
    /// The main editor, it has the means to create modules, connections and produce the resulting audio
    Patcher patcher;
    
    /// The Viewport component that presents the view of mainPatcher to allow scrolling
    juce::Viewport viewport;
    
    struct FileMenuModel : juce::MenuBarModel {
        FileMenuModel(FileManager& manager) : manager(manager) {}
        
        juce::StringArray getMenuBarNames() override { return {"File"}; }
        
        void menuItemSelected (int menuItemID, int topLevelMenuIndex) override {}

        juce::PopupMenu getMenuForIndex (int topLevelMenuIndex, const juce::String& menuName) override {
            if (menuName == "File") {
                juce::PopupMenu menu;
                
                menu.addItem("Save", [&] () { manager.save(); });
                menu.addItem("Save As...", [&] () { manager.saveAs(); });
                menu.addItem("Open...", [&] () { manager.open(); });
                
                return menu;
            }
            return {};
        }
        
    private:
        FileManager& manager;
    };
    
    FileMenuModel fileMenuModel;
    juce::MenuBarComponent fileMenu;
    
    /// A simple button to change the patch-cord drawing method (just because)
    PhiSliderButton patchCordTypeButton;
    
    /// A simple button to toggle between inlet/outlet names being hinted or labeled
    PhiSliderButton showPortLabelsButton;
    
    /// The rectangle that constrains the top bar on the window
    juce::Rectangle<int> topBarBounds;
    
    //==============================================================================
    
    void fileLoaded(juce::File) override;
    void fileSaved(juce::File) override;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
