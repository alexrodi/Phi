/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../lib/ModuleBox.h"
#include "../lib/Connections.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent   : public AudioAppComponent
{
//==============================================================================
public:
    
    MainComponent();
    ~MainComponent();

    // Audio =======================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (Graphics& g) override;
    void resized() override;
    
void mouseDown(const MouseEvent& e) override;

//==============================================================================
private:
    // Our LookAndFeel instance for the MainComponent
    // this maintains certain colors in the window but any module can surpass it with its own
    LookAndFeel_V4 lookandfeel;

    // The array of modules
    OwnedArray<ModuleBox> modules;
    
    // The list of selected modules
    // gets passed to each module because they subscribe themselves to the list
    // they also use it for dragging all the selected modules at once
    SelectedItemSet<ModuleBox*> selectedModules;
    
    // Right click menu and submenus
    PopupMenu rightClickMenu;
    PopupMenu modulesSubMenu;
    
    // The connections class
    Connections connections;
    
    void registerInletsAndOutlets(ModuleBox *module);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
