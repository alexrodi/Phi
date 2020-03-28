/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Some platforms require permissions to open input channels so request that here
    if (RuntimePermissions::isRequired (RuntimePermissions::recordAudio)
        && ! RuntimePermissions::isGranted (RuntimePermissions::recordAudio))
    {
        RuntimePermissions::request (RuntimePermissions::recordAudio,
                                     [&] (bool granted) { if (granted)  setAudioChannels (2, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }
    
    // Set the colors of PopupMenu
    setLookAndFeel(&lookandfeel);
    lookandfeel.setColour(PopupMenu::backgroundColourId, Colours::darkgrey.darker());
    lookandfeel.setColour(PopupMenu::highlightedBackgroundColourId, Colour::greyLevel(0.2));
    
    sendLookAndFeelChange();
    
    // Menu and submenus content
    // Submenus must be filled before the main
    modulesSubMenu.addItem (1, "Impulse");
    rightClickMenu.addSubMenu ("Add Module...", modulesSubMenu);
    
    // Apply our colors to the menu
    rightClickMenu.setLookAndFeel(&lookandfeel);
    
    addAndMakeVisible(connections);
    
    setSize (600, 400);
}

MainComponent::~MainComponent()
{
    setLookAndFeel(nullptr);
    
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    // For more details, see the help for AudioProcessor::prepareToPlay()
}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    // Your audio-processing code goes here!

    // For more details, see the help for AudioProcessor::getNextAudioBlock()

    // Right now we are not producing any data, in which case we need to clear the buffer
    // (to prevent the output of random noise)
    bufferToFill.clearActiveBufferRegion();
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    g.fillAll (Colours::darkgrey);
}

void MainComponent::resized()
{
    connections.setBounds(getLocalBounds());
}

// Registers all inlets and outlets with the connections component
void MainComponent::registerInletsAndOutlets(ModuleBox *module) {
    
    OwnedArray<phi_Inlet>& inlets = module->module.inlets;
    for (phi_Inlet* inlet : inlets)
    {
        inlet->inletID = connections.registerInlet(inlet);
        inlet->addActionListener(&connections);
    }
    
    OwnedArray<phi_Outlet>& outlets = module->module.outlets;
    for (phi_Outlet* outlet : outlets)
    {
        outlet->outletID = connections.registerOutlet(outlet);
        outlet->addActionListener(&connections);
    }
    
}

void MainComponent::mouseDown(const MouseEvent& e)
{
    // Deselect all modules when clicking the window
    selectedModules.deselectAll();
    
    // Right Click
    if (e.mods.isRightButtonDown()){
        
        // Displays the menu and returns the ID of the selected item (0 if clicked outside)
        const int result = rightClickMenu.show();
        
        // There's only one possible result for now (1 - Impulse)
        if (result)
        {
            // Create the module
            ModuleBox* module = new ModuleBox(&selectedModules);
            
            registerInletsAndOutlets(module);
            
            // Add it to the array
            modules.add(module);
            
            // Display and set its (initial) bounds
            addAndMakeVisible(module);
            module->setBounds(e.x, e.y, 400, 200);
            module->addActionListener(&connections);
        }
    }
}
