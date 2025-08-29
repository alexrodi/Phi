
#pragma once

#include "../State.h"
#include "../FileManager.h"
#include "Patcher.h"
#include "PhiLookAndFeel.h"
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
    const int topBarHeight = 38;
    
    PhiLookAndFeel lookandfeel;
    
    //==============================================================================
    State& state;
    
    /// The main editor, it has the means to create modules, connections and produce the resulting audio
    Patcher patcher;
    
    /// The Viewport component that presents the view of mainPatcher to allow scrolling
    juce::Viewport viewport;
    
    struct MenuBar : juce::MenuBarModel {
        MenuBar(MainComponent& owner, FileManager& fileManager) :
        owner(owner),
        fileManager(fileManager),
        component(this)
        {
            owner.addAndMakeVisible(component);
        }
        
        void setBounds(juce::Rectangle<int> bounds) {
            component.setBounds(bounds);
        };
        
        juce::StringArray getMenuBarNames() override { return {"File", "Theme"}; }
        
        void menuItemSelected (int menuItemID, int topLevelMenuIndex) override {}

        juce::PopupMenu getMenuForIndex (int topLevelMenuIndex, const juce::String& menuName) override {
            if (menuName == "File") {
                juce::PopupMenu menu;
                
                menu.addItem("Open...", [&] () { fileManager.open(); });
                menu.addItem("Save", [&] () { fileManager.save(); });
                menu.addItem("Save As...", [&] () { fileManager.saveAs(); });
                
                return menu;
            } else if (menuName == "Theme") {
                juce::PopupMenu menu;
                
                // TODO: should be an iterative method
                using Theme = PhiTheme::Palettes;
                menu.addItem("Default Classic", [&] () { setTheme(Theme::DefaultClassic); });
                menu.addItem("Default Dark",    [&] () { setTheme(Theme::DefaultDark); });
                menu.addItem("Solaris Light",   [&] () { setTheme(Theme::SolarisLight); });
                menu.addItem("Midnight Blue",   [&] () { setTheme(Theme::MidnightBlue); });
                menu.addItem("Vintage Analog",  [&] () { setTheme(Theme::VintageAnalog); });
                menu.addItem("Nordic Noir",     [&] () { setTheme(Theme::NordicNoir); });
                menu.addItem("Cyberpunk Neon",  [&] () { setTheme(Theme::CyberpunkNeon); });
                
                return menu;
            }
            return {};
        }
        
    private:
        MainComponent& owner;
        FileManager& fileManager;
        juce::MenuBarComponent component;
        
        void setTheme(const PhiTheme& theme) {
            owner.lookandfeel.setTheme(theme, true);
            
            owner.patcher.setModulesTheme(theme);
            
            owner.sendLookAndFeelChange();
            owner.repaint();
        }
    };
    
    MenuBar menuBar;
    
    /// A simple button to change the patch-cord drawing method (just because)
    PhiSliderButton patchCordTypeButton;
    
    /// A simple button to toggle between inlet/outlet names being hinted or labeled
    PhiSliderButton showPortLabelsButton;
    
    //==============================================================================
    
    void fileLoaded(juce::File) override;
    void fileSaved(juce::File) override;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
