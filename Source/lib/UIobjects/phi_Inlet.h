/*
  ==============================================================================

    phi_Inlet.h
    Created: 18 Feb 2020 11:19:12pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class phi_Inlet    : public Component,
                     public ActionBroadcaster
{
public:
    phi_Inlet();
    ~phi_Inlet();

    void paint (Graphics&) override;
    void resized() override;

private:
    Rectangle<float> viewport;
    
    String getCenterAsRectangleString (const MouseEvent&);
    String getMouseAsRectangleString (const MouseEvent&);
    
    void mouseDown(const MouseEvent&) override;
    void mouseUp(const MouseEvent&) override;
    void mouseDrag(const MouseEvent&) override;
    void mouseEnter(const MouseEvent&) override;
    void mouseExit(const MouseEvent&) override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (phi_Inlet)
};
