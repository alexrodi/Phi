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

#include "PlugHandler.h"
#include "HoverPopup.h"

//==============================================================================
namespace PlugOptions
{
    extern bool drawName;
}

/// A base class for Inlet and Outlet - This is a UI class for handling connection and dragging behaviors, it also draws the Inlet/Outlet itself
class Plug    : public Component,
                public HoverPopupClient,
                public PlugEventEmitter
{
public:
    /// To construct a plug you must specify a mode and a name
    Plug(PlugType, const String&);
    
    /// Move Constructor
    Plug(Plug&& other) noexcept;
    
    ~Plug() {};

    void paint (Graphics&) override;
    void resized() override;
    
    /// Sets the ID that the Plug will use when broadcasting (assigned on module creation)
    void setID(PlugID);
    
    PlugType getMode() const {
        return mode;
    }
    
    PlugID getID() const {
        return plugID;
    }

private:
    struct PlugColors { Colour inner, outer; };
    
    const int SIZE = 12;
    const int STROKE_WIDTH = 3;
    
    /// The plug's name - `Inlet` or `Outlet`
    PlugType mode;
    
    /// The ID of this plug, containing the moduleID and plugID, that the Plug will use when broadcasting
    PlugID plugID;
    
    /// The Plug's colors
    PlugColors colors;
    
    /// This Plug's bounds
    Rectangle<float> bounds;
    /** The bounds for the Plug Name (if one is displayed)
     @see PlugOptions::drawName
     */
    Rectangle<int> nameBounds;
    
    /// A justification object to use when drawing the Name text
    Justification::Flags nameJustification;
    
    bool canFitText;
    
    void mouseDown(const MouseEvent&) override;
    void mouseUp(const MouseEvent&) override;
    void mouseDrag(const MouseEvent&) override;
    juce::Point<float> hoverPopupPosition() override;
    juce::String getPopupText() override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Plug)
};

/// A Plug in Inlet mode (other functionality might be added here)
class InletPlug : public Plug
{
public:
    explicit InletPlug(const String& name) : Plug(PlugType::Inlet, name)
    {}
};

/// A Plug in Outlet mode (other functionality might be added here)
class OutletPlug : public Plug
{
public:
    explicit OutletPlug(const String& name) : Plug(PlugType::Outlet, name)
    {}
};
