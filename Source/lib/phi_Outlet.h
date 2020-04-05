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
                     public ActionBroadcaster,
                     public DragAndDropTarget
{
public:
    phi_Outlet();
    ~phi_Outlet();

    void paint (Graphics&) override;
    void resized() override;
    
    void setId(std::pair<uint32, int>);

private:
    
    uint32 moduleID;
    int outletID;
    
    Rectangle<float> viewport;
    
    String getStringId();
    
    void mouseDown(const MouseEvent&) override;
    void mouseUp(const MouseEvent&) override;
    void mouseDrag(const MouseEvent&) override;
    
    bool isInterestedInDragSource (const SourceDetails&) override;
    void itemDropped (const SourceDetails&) override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (phi_Outlet)
};
