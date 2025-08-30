
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
    
    struct MenuBar : juce::MenuBarComponent {
        MenuBar(MainComponent& owner, FileManager& fileManager) :
        model(owner, fileManager)
        {
            setModel(&model);
        }
        
        ~MenuBar() {
            setModel(nullptr);
        }
        
    private:
        struct Model : juce::MenuBarModel {
            Model(MainComponent& owner, FileManager& fileManager) : fileManager(fileManager), owner(owner) {}
            
            juce::StringArray getMenuBarNames() override { return {"File", "Theme"}; }

            juce::PopupMenu getMenuForIndex (int topLevelMenuIndex, const juce::String& menuName) override {
                if (menuName == "File") {
                    juce::PopupMenu menu;
                    
                    menu.addItem("Open...",    [&] () { fileManager.open(); });
                    menu.addItem("Save",       [&] () { fileManager.save(); });
                    menu.addItem("Save As...", [&] () { fileManager.saveAs(); });
                    
                    return menu;
                } else if (menuName == "Theme") {
                    juce::PopupMenu menu;
                    
                    // TODO: should be an iterative method
                    using Theme = PhiTheme::Palettes;
                    menu.addItem("Default", [&] () { owner.setTheme(Theme::Default); });
                    menu.addItem("Pop",     [&] () { owner.setTheme(Theme::Pop); });
                    menu.addItem("Boulder", [&] () { owner.setTheme(Theme::Boulder); });
                    menu.addItem("Neon",    [&] () { owner.setTheme(Theme::Neon); });
                    
                    return menu;
                }
                return {};
            }
            
            void menuItemSelected (int menuItemID, int topLevelMenuIndex) override {}
            
        private:
            FileManager& fileManager;
            MainComponent& owner;
        };
        
        Model model;
    };
    
    MenuBar menuBar;
    
    /// A simple button to change the patch-cord drawing method (just because)
    PhiSliderButton patchCordTypeButton;
    
    /// A simple button to toggle between inlet/outlet names being hinted or labeled
    PhiSliderButton showPortLabelsButton;
    
    void setTheme(const PhiTheme& theme){
        state.setTheme(theme);
    }
    
    //==============================================================================
    
    void fileLoaded(juce::File) override;
    void fileSaved(juce::File) override;
    void themeChanged(const PhiTheme& theme) override {
        lookandfeel.setTheme(theme, true);
        
        sendLookAndFeelChange();
        repaint();
    }
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
