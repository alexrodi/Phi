/*
  ==============================================================================

    FileManager.h
    Created: 28 Aug 2025 5:15:32pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

#include "State.h"

struct FileManager {
    FileManager(State& state) : state(state) {}
    
    void save(std::function<void()> callback = {}) {
        if (currentlyOpenFile.existsAsFile()) {
            state.save(currentlyOpenFile);
            if (callback) callback();
        } else {
            saveAs(callback);
        }
    }
    
    void saveAs(std::function<void()> callback = {}) {
        using Flags = juce::FileBrowserComponent::FileChooserFlags;
        
        chooser = std::make_unique<juce::FileChooser>("Save As...", currentlyOpenFile, "*.phi");
        int flags = juce::FileBrowserComponent::saveMode + juce::FileBrowserComponent::warnAboutOverwriting;
        
        chooser->launchAsync(flags, [&, callback] (const juce::FileChooser& chooser) {
            if (auto file = chooser.getResult(); file.create().wasOk()) {
                if (file.getFileExtension() != ".phi")
                    file.withFileExtension(".phi");
                
                state.save(file);
                currentlyOpenFile = file;
                
                if (callback) callback();
            }
        });
    }
    
    void open() {
        askToSaveThen([&] () {
            chooser = std::make_unique<juce::FileChooser>("Open...", currentlyOpenFile, "*.phi");
            int flags = juce::FileBrowserComponent::openMode + juce::FileBrowserComponent::canSelectFiles;
            
            chooser->launchAsync(flags, [&] (const juce::FileChooser& chooser) {
                if (auto file = chooser.getResult(); file.existsAsFile()) {
                    state.load(file);
                    currentlyOpenFile = file;
                }
            });
        });
    }
    
    template<class Callback>
    void askToSaveThen(Callback callbackIfNotCanceled) {
        if (!state.isDirty()) { // No need to save
            callbackIfNotCanceled();
            return;
        }
        
        auto filename = currentlyOpenFile.existsAsFile() ? currentlyOpenFile.getFileNameWithoutExtension() : "Untitled";
        juce::NativeMessageBox::showAsync(juce::MessageBoxOptions()
                .withIconType(juce::MessageBoxIconType::QuestionIcon)
                .withMessage("Save changes to " + filename + "?")
                .withButton ("Save")
                .withButton ("Don't Save")
                .withButton ("Cancel"),
            [&, callbackIfNotCanceled] (int result) {
                if (result == 0) // User wants to save
                    save(callbackIfNotCanceled);
                else if (result == 1) // User doesn't want to save
                    callbackIfNotCanceled();
            }
        );
    }
    
private:
    State& state;
    std::unique_ptr<juce::FileChooser> chooser;
    juce::File currentlyOpenFile;
};

