
#pragma once

#include "../State.h"
#include "PhiColours.h"
#include "Patcher.h"
#include "component/PhiSliderButton.h"

//==============================================================================
/// The top-most component that includes all the content
struct MainComponent : juce::Component,
                       State::Listener

{
//==============================================================================
    explicit MainComponent(State&);
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
    
    // TODO: make a top bar in the window with all the options (also mac menu?)
    struct FileMenuModel : juce::MenuBarModel {
        FileMenuModel(State& state) : state(state) {}
        
        juce::StringArray getMenuBarNames() override { return {"File"}; }

        
        juce::PopupMenu getMenuForIndex (int topLevelMenuIndex, const juce::String& menuName) override {
            if (menuName == "File") {
                juce::PopupMenu menu;
                
                menu.addItem("Save", [&] () {
                    if (currentlyOpenFile.existsAsFile())
                        state.save(currentlyOpenFile);
                    else
                        saveAs();
                });
                
                menu.addItem("Save As...", [&] () {saveAs();});
                
                menu.addItem("Open...", [&] () {
                    chooser = std::make_unique<juce::FileChooser>("Open...", currentlyOpenFile, "*.phi");
                    int flags = juce::FileBrowserComponent::openMode + juce::FileBrowserComponent::canSelectFiles;
                    
                    chooser->launchAsync(flags, [&] (const juce::FileChooser& chooser) {
                        auto file = chooser.getResult();
                        
                        state.load(file);
                        currentlyOpenFile = file;
                    });
                });
                
                return menu;
            }
            return {};
        }
        
        void menuItemSelected (int menuItemID, int topLevelMenuIndex) override {}
        
    private:
        State& state;
        std::unique_ptr<juce::FileChooser> chooser;
        juce::File currentlyOpenFile;
        
        void saveAs() {
            using Flags = juce::FileBrowserComponent::FileChooserFlags;
            
            chooser = std::make_unique<juce::FileChooser>("Save As...", currentlyOpenFile, "*.phi");
            int flags = juce::FileBrowserComponent::saveMode + juce::FileBrowserComponent::warnAboutOverwriting;
            
            chooser->launchAsync(flags, [&] (const juce::FileChooser& chooser) {
                if (auto file = chooser.getResult(); file.create().wasOk()) {
                    if (file.getFileExtension() != ".phi")
                        file.withFileExtension(".phi");
                    
                    state.save(file);
                    currentlyOpenFile = file;
                }
            });
        }
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
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
