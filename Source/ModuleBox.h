/*
  ==============================================================================

    ModuleBox.h
    Created: 8 Feb 2020 12:28:45pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "modules/phi_Impulse.h"

//==============================================================================
/*
*/
class ModuleBox    : public Component,
                     public ChangeListener,
                     public Button::Listener,
                     public ComponentDragger,
                     public ComponentBoundsConstrainer
{
public:
    ModuleBox(SelectedItemSet<ModuleBox*>*);
    ~ModuleBox();

    void paint (Graphics&) override;
    void resized() override;
    
    void mouseDown(const MouseEvent& e) override;
    void mouseUp(const MouseEvent& e) override;
    void mouseDrag(const MouseEvent& e) override;
    
    void changeListenerCallback (ChangeBroadcaster* source) override;

private:
    phi_Impulse module;
    Rectangle<float> moduleRectangle;
    
    class PowerButton : public Button
    {
    public:
        PowerButton() :
        Button("Enabled")
        {
            setPaintingIsUnclipped(true);
            setClickingTogglesState(true);
            setToggleState(true, NotificationType());
        }
    private:
        void paintButton (Graphics& g,
                          bool shouldDrawButtonAsHighlighted,
                          bool shouldDrawButtonAsDown) override
        {
            Rectangle<float> buttonRect = getLocalBounds().reduced(1).toFloat();
            g.setColour(Colours::grey);
            g.drawRect(buttonRect, 1);
            g.setColour(findColour(Slider::thumbColourId));
            g.fillRect(buttonRect.reduced(1));
        }
        
    } powerButton;
    
    void startDraggingSelected(const MouseEvent& e);
    void dragSelected(const MouseEvent& e);
    
    
    void buttonStateChanged (Button*) override;
    void buttonClicked (Button*) override {};

    ResizableCornerComponent resizer;
    SelectedItemSet<ModuleBox*>* moduleSelection;
    bool selectResult;
    bool isSelected;
    
    int contentPadding;
    int headerHeight;
    Rectangle<float> headerLine;
    
    LookAndFeel_V4 lookandfeel;
    
    //void buttonStateChanged (Button*) override {}

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ModuleBox)
};
