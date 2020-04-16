/*
  ==============================================================================

    Outlet.h
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
namespace OutletOptions
{
    extern bool drawName;
}

class Outlet    : public Component,
                     public ActionBroadcaster,
                     public DragAndDropTarget,
                     public SettableTooltipClient
{
public:
    explicit Outlet(const String&);
    ~Outlet();

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
    
    bool canFitText;
    
    String getStringId();
    
    void mouseDown(const MouseEvent&) override;
    void mouseUp(const MouseEvent&) override;
    void mouseDrag(const MouseEvent&) override;
    
    bool isInterestedInDragSource (const SourceDetails&) override;
    void itemDropped (const SourceDetails&) override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Outlet)
};
