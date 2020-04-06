/*
  ==============================================================================

    module_Output.h
    Created: 5 Apr 2020 10:13:32pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../Module.h"

//==============================================================================
/*
*/
class module_Output    : public Module
{
public:
    module_Output();
    ~module_Output();
    
    // Audio =======================================================================
    void prepareToPlay (double, int) override;
    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;
    void releaseResources() override;

    void paint (Graphics&) override;
    void wasResized(Rectangle<int>) override;
    
    void lookAndFeelChanged() override;

private:
    std::unique_ptr<Drawable> speakerImage;
    
    Colour colour;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (module_Output)
};
