/*
  ==============================================================================

    phi_Outlet.h
    Created: 18 Feb 2020 11:19:27pm
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
class phi_Outlet    : public Component,
                     public ActionBroadcaster,
                     public DragAndDropTarget
{
public:
    enum NamePosition
    {
        Left,
        Right,
        Above,
        Below
    } namePosition;
    
    explicit phi_Outlet(String, NamePosition = NamePosition::Above);
    ~phi_Outlet();

    void paint (Graphics&) override;
    void resized() override;
    
    String name;
    
    void setId(std::pair<uint32, int>);

private:
    
    uint32 moduleID;
    int outletID;
    
    Rectangle<float> outletBounds;
    Rectangle<int> nameBounds;
    
    Justification::Flags nameJustification;
    
    bool drawText;
    
    String getStringId();
    
    void mouseDown(const MouseEvent&) override;
    void mouseUp(const MouseEvent&) override;
    void mouseDrag(const MouseEvent&) override;
    
    bool isInterestedInDragSource (const SourceDetails&) override;
    void itemDropped (const SourceDetails&) override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (phi_Outlet)
};
