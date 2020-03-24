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
//==================================================================================
public:
    ModuleBox(SelectedItemSet<ModuleBox*>*);
    ~ModuleBox();

    void paint (Graphics&) override;
    void resized() override;
    
    // The hosted module
    phi_Impulse module;

//==================================================================================
private:
   // Our LookAndFeel instance for this module box
   LookAndFeel_V4 lookandfeel;
   
   // Layout
   int contentPadding;
   int headerHeight;
   Rectangle<float> moduleRectangle;
   Rectangle<float> headerLine;
   Rectangle<float> nameRectangle;
    
    //==================================================================================
    // Our Power button (this might be transported to its own file)
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
        // Override the button's paint method
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
    
    
    // Module resizer component
    ResizableCornerComponent resizer;
    
    // Pointer to the MainComponent's selection set
    SelectedItemSet<ModuleBox*>* moduleSelection;
    // Variables to manage its own selection
    bool selectResult;
    bool isSelected;
    
    //==================================================================================
    // Dragger wrappers for selected items
    void startDraggingSelected(const MouseEvent& e);
    void dragSelected(const MouseEvent& e);
    
    // Button Listeners
    void buttonStateChanged (Button*) override;
    void buttonClicked (Button*) override {};
    
    void mouseDown(const MouseEvent& e) override;
    void mouseUp(const MouseEvent& e) override;
    void mouseDrag(const MouseEvent& e) override;
    
    void changeListenerCallback (ChangeBroadcaster* source) override;
    
    
    //void buttonStateChanged (Button*) override {}

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ModuleBox)
};
