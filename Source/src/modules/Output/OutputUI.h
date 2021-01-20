/*
  ==============================================================================

    OutputUI.h
    Created: 5 Apr 2020 10:13:32pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

///@cond
#include <JuceHeader.h>
///@endcond
#include "../../ModuleUI.h"

//==============================================================================
/*
*/
class OutputUI    : public ModuleUI
{
public:
    OutputUI(ModuleProcessor&);
    ~OutputUI();

    void paint (Graphics&) override;
    void wasResized(Rectangle<int>) override;

    void lookAndFeelChanged() override;

private:
    std::unique_ptr<Drawable> speakerImage;

    Colour colour;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OutputUI)
};
