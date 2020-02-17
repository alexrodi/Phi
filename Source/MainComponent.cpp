/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    setSize (600, 400);
    
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
    
    setLookAndFeel(&lookandfeel);
    lookandfeel.setColour(PopupMenu::backgroundColourId, Colours::darkgrey.darker());
    lookandfeel.setColour(PopupMenu::highlightedBackgroundColourId, Colour::greyLevel(0.2));
    
    sendLookAndFeelChange();
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
}

void MainComponent::mouseDown(const MouseEvent& e)
{
    selectedModules.deselectAll();
    
    if (e.mods.isRightButtonDown()){
        PopupMenu subMenu;
        subMenu.addItem (1, "Impulse");

        PopupMenu mainMenu;
        mainMenu.addSubMenu ("Add Module...", subMenu);
        
        mainMenu.setLookAndFeel(&lookandfeel);
        
        const int result = mainMenu.show();
        
        if (result){
            ModuleBox* module = new ModuleBox(&selectedModules);
            modules.add(module);
            addAndMakeVisible(module);
            module->setBounds(e.x, e.y, 350, 200);
        }
    }
}
