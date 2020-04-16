/*
  ==============================================================================

    Plugs.h
    Created: 16 Apr 2020 11:08:52pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
namespace PlugOptions
{
    extern bool drawName;
}
/// A base class for Inlet and Outlet
class Plug    : public Component,
                public ActionBroadcaster,
                public DragAndDropTarget,
                public SettableTooltipClient
{
public:
    enum Mode { Inlet, Outlet } mode;
    
    Plug(Mode, const String&);
    ~Plug();

    void paint (Graphics&) override;
    void resized() override;
    
    String name;
    
    void setId(std::pair<uint32, int>);

private:
    uint32 moduleID;
    int ownID;
    
    typedef std::pair<Colour, Colour> ColourPair;
    typedef std::pair<String, String> StringPair;
    
    ColourPair colors;
    StringPair modeString;
    
    Rectangle<float> bounds;
    Rectangle<int> nameBounds;
    
    Justification::Flags nameJustification;
    
    bool canFitText;
    
    String getStringId();
    
    void mouseDown(const MouseEvent&) override;
    void mouseUp(const MouseEvent&) override;
    void mouseDrag(const MouseEvent&) override;
    
    bool isInterestedInDragSource (const SourceDetails&) override;
    void itemDropped (const SourceDetails&) override;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Plug)
};

class Inlet : public Plug
{
public:
    explicit Inlet(const String& name) : Plug(Plug::Inlet, name)
    {}
};

class Outlet : public Plug
{
public:
    explicit Outlet(const String& name) : Plug(Plug::Outlet, name)
    {}
};
