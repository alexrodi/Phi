/*
  ==============================================================================

    MainPatcher.h
    Created: 29 Mar 2020 12:51:46am
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ModuleBox.h"
#include "Connections.h"
#include "modules/Modules.h"

//==============================================================================
/*
*/
class MainPatcher    : public Component,
                       public AudioProcessorGraph,
                       public DragAndDropContainer
{
public:
    MainPatcher();
    ~MainPatcher();
    
    // Audio =======================================================================
    void prepareToPlay (double, int) override;
    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;
    void releaseResources() override;

    void paint (Graphics&) override;
    void resized() override;
    
    void togglePatchCordType();

private:
    
    // The array of modules
    OwnedArray<ModuleBox> modules;
    
    // The list of selected modules
    // gets passed to each module because they subscribe themselves to the list
    // they also use it for dragging all the selected modules at once
    SelectedItemSet<ModuleBox*> selectedModules;
    
    // The connections class
    Connections connections;
    
    // Right click menu and submenus
    PopupMenu rightClickMenu;
    PopupMenu modulesSubMenu;
    
    void registerInletsAndOutlets(std::unique_ptr<Module>&);
    
    template <class moduleClass>
    void createModule(Point<float>);
    
    void mouseDown(const MouseEvent& e) override;
    
    
    const String getName() const override {return String("");};
    double getTailLengthSeconds() const override {return 0.0f;};
    bool acceptsMidi() const override {return false;};
    bool producesMidi() const override {return false;};
    AudioProcessorEditor* createEditor() override {return nullptr;};
    bool hasEditor() const override {return false;};
    int getNumPrograms() override {return 0;};
    int getCurrentProgram() override {return 0;};
    void setCurrentProgram (int index) override {};
    const String getProgramName (int index) override {return String("");};
    void changeProgramName (int index, const String& newName) override {};
    void getStateInformation (juce::MemoryBlock& destData) override {};
    void setStateInformation (const void* data, int sizeInBytes) override {};
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainPatcher)
};
