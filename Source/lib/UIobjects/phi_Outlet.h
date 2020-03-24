/*
  ==============================================================================

    phi_Outlet.h
    Created: 18 Feb 2020 11:19:27pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class phi_Outlet    : public Component,
                      public ActionBroadcaster
{
public:
    phi_Outlet();
    ~phi_Outlet();

    void paint (Graphics&) override;
    void resized() override;
    
private:
    Rectangle<float> viewport;
    
    void mouseDown(const MouseEvent& e) override;
    void mouseUp(const MouseEvent& e) override;
    void mouseDrag(const MouseEvent& e) override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (phi_Outlet)
};
