
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
        fileManager(fileManager),
        model(*this) {
            setModel(&model);
        }
        
        ~MenuBar() {
            setModel(nullptr);
        }
        
    private:
        struct Model : juce::MenuBarModel {
            Model(MenuBar& owner) : owner(owner) {}
            
            juce::StringArray getMenuBarNames() override { return {"File", "Theme"}; }

            juce::PopupMenu getMenuForIndex (int topLevelMenuIndex, const juce::String& menuName) override {
                if (menuName == "File") {
                    juce::PopupMenu menu;
                    
                    menu.addItem("Open...",    [&] () { owner.fileManager.open(); });
                    menu.addItem("Save",       [&] () { owner.fileManager.save(); });
                    menu.addItem("Save As...", [&] () { owner.fileManager.saveAs(); });
                    
                    return menu;
                } else if (menuName == "Theme") {
                    juce::PopupMenu menu;
                    
                    // TODO: should be an iterative method
                    using Theme = PhiTheme::Palettes;
                    menu.addItem("Default", [&] () { owner.setTheme(Theme::Default); });
                    menu.addItem("Slate",   [&] () { owner.setTheme(Theme::Slate); });
                    menu.addItem("Dusk",    [&] () { owner.setTheme(Theme::Dusk); });
                    menu.addItem("Earth",   [&] () { owner.setTheme(Theme::Earth); });
                    menu.addItem("Granite", [&] () { owner.setTheme(Theme::Granite); });
                    
                    return menu;
                }
                return {};
            }
            
            void menuItemSelected (int menuItemID, int topLevelMenuIndex) override {}
            
        private:
            MenuBar& owner;
        };
        
        FileManager& fileManager;
        Model model;
        
        void setTheme(const PhiTheme& theme) {
            static_cast<MainComponent*>(getParentComponent())->setTheme(theme);
        }
    };
    
    MenuBar menuBar;
    
    /// A simple button to change the patch-cord drawing method (just because)
    PhiSliderButton patchCordTypeButton;
    
    /// A simple button to toggle between inlet/outlet names being hinted or labeled
    PhiSliderButton showPortLabelsButton;
    
    void setTheme(const PhiTheme& theme){
        lookandfeel.setTheme(theme, true);
        patcher.setModulesTheme(theme);
        
        sendLookAndFeelChange();
        repaint();
    }
    
    //==============================================================================
    
    void fileLoaded(juce::File) override;
    void fileSaved(juce::File) override;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
