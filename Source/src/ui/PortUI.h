/*
  ==============================================================================

    PortUI.h
    Created: 16 Apr 2020 11:08:52pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

#include "../State.h"
#include "component/HoverPopup.h"

//==============================================================================

/// A base class for inlets and outlets
struct PortUI : juce::Component,
                HoverPopupClient
{
    /// To construct a plug you must specify a type and a name
    PortUI(PortType, const juce::String&);
    
    ~PortUI() {};

    void paint (juce::Graphics&) override;
    void resized() override;
    
    PortType getType() const { return type; }
    
    void showLabel(ShowPortLabels show) { drawName = show == ShowPortLabels::On; }

private:
    struct PortColors { juce::Colour inner, outer; };
    
    const int SIZE = 12;
    const int STROKE_WIDTH = 3;
    
    /// The plug's type - `Inlet` or `Outlet`
    PortType type;
    
    /// The Port's colors
    PortColors colors;
    
    /// This Port's bounds
    juce::Rectangle<float> bounds;
    /** The bounds for the Port Name (if one is displayed)
     @see PortOptions::drawName
     */
    juce::Rectangle<int> nameBounds;
    
    /// A justification object to use when drawing the Name text
    juce::Justification::Flags nameJustification;
    
    bool canFitText = false, drawName = false;
    
    void mouseDown(const juce::MouseEvent&) override;
    void mouseUp(const juce::MouseEvent&) override;
    void mouseDrag(const juce::MouseEvent&) override;
    juce::Point<float> hoverPopupPosition() override;
    juce::String getPopupText() override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PortUI)
};

/// A Port in Inlet mode (other functionality might be added here)
struct InletUI : PortUI
{
    explicit InletUI(const juce::String& name) : PortUI(PortType::Inlet, name) {}
};

/// A Port in Outlet mode (other functionality might be added here)
struct OutletUI : PortUI
{
    explicit OutletUI(const juce::String& name) : PortUI(PortType::Outlet, name) {}
};
