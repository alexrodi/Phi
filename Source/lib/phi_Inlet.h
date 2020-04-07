/*
  ==============================================================================

    phi_Inlet.h
    Created: 18 Feb 2020 11:19:12pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

///@cond
#include <JuceHeader.h>
///@endcond

//==============================================================================
/*
*/
class phi_Inlet    : public Component,
                     public ActionBroadcaster,
                     public DragAndDropTarget
{
public:
    phi_Inlet();
    ~phi_Inlet();

    void paint (Graphics&) override;
    void resized() override;
    
    void setId(std::pair<uint32, int>);

private:
    uint32 moduleID;
    int inletID;
    
    Rectangle<float> viewport;
    
    String getStringId();
    
    void mouseDown(const MouseEvent&) override;
    void mouseUp(const MouseEvent&) override;
    void mouseDrag(const MouseEvent&) override;
    
    bool isInterestedInDragSource (const SourceDetails&) override;
    void itemDropped (const SourceDetails&) override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (phi_Inlet)
};
