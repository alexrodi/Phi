/*
  ==============================================================================

    Plugs.h
    Created: 16 Apr 2020 11:08:52pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

///@cond
#include <JuceHeader.h>
///@endcond

//==============================================================================
namespace PlugOptions
{
    extern bool drawName;
}
/// A base class for Inlet and Outlet - This is a UI class for handling connection and dragging behaviors, it also draws the Inlet/Outlet itself
class Plug    : public Component,
                public ActionBroadcaster,
                public DragAndDropTarget,
                public SettableTooltipClient
{
public:
    /// This Class may exist in one of two modes
    enum Mode { Inlet, Outlet } mode;
    
    /// To construct a plug you must specify a mode and a name
    Plug(Mode, const String&);
    ~Plug();

    void paint (Graphics&) override;
    void resized() override;
    
    /// The plug's name - to be displayed or hinted
    String name;
    
    /// Sets the ID that the Plug will use when broadcasting (assigned on module creation)
    void setId(std::pair<uint32, int>);

private:
    typedef std::pair<Colour, Colour> ColourPair;
    typedef std::pair<String, String> StringPair;
    
    /// The ID belonging to the parent module, that the Plug will use when broadcasting
    uint32 moduleID;
    /// The ID of this Plug (unique only to its own type within a module, e.g. inlets{1,2,3}, outlets{1,2}), that the Plug will use when broadcasting
    int ownID;
    
    /// The Plug's colors - {inner, outer}
    ColourPair colors;
    /** A utility pair with strings for broadcasting - {ownMode, otherMode}
     e.g. if the case of Mode::Inlet, this will be: {"inlet", "outlet"}
     */
    StringPair modeString;
    
    /// This Plug's bounds
    Rectangle<float> bounds;
    /** The bounds for the Plug Name (if one is displayed)
     @see PlugOptions::drawName
     */
    Rectangle<int> nameBounds;
    
    /// A justification object to use when drawing the Name text
    Justification::Flags nameJustification;
    
    bool canFitText;
    
    /// Returns this Plug's ID as a String (for broadcasting) -> "moduleID>ownID"
    String getStringId();
    
    void mouseDown(const MouseEvent&) override;
    void mouseUp(const MouseEvent&) override;
    void mouseDrag(const MouseEvent&) override;
    
    /** Callback for claiming interest in the thing being dragged.
     An inlet is interested in a dragged outlet but not another inlet,
     this also returns false if the drag source comes from the same module this Plug belongs to
     */
    bool isInterestedInDragSource (const SourceDetails&) override;
    /// Called when a drag source is dropped here and interest was claimed in isInterestedInDragSource()
    void itemDropped (const SourceDetails&) override;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Plug)
};

/// A Plug in Inlet mode (other functionality might be added here)
class Inlet : public Plug
{
public:
    explicit Inlet(const String& name) : Plug(Plug::Inlet, name)
    {}
};

/// A Plug in Outlet mode (other functionality might be added here)
class Outlet : public Plug
{
public:
    explicit Outlet(const String& name) : Plug(Plug::Outlet, name)
    {}
};
