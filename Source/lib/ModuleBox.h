/*
  ==============================================================================

    ModuleBox.h
    Created: 8 Feb 2020 12:28:45pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

///@cond
#include <JuceHeader.h>
///@endcond
#include "modules/Modules.h"

#define HEADER_HEIGHT 27
#define CONTENT_PADDING 10

//==============================================================================
/*
*/
class ModuleBox    : public Component,
                     public ChangeListener,
                     public Button::Listener,
                     public ComponentDragger,
                     public ComponentBoundsConstrainer,
                     public ActionBroadcaster
{
//==================================================================================
public:
    ModuleBox(Module*, SelectedItemSet<ModuleBox*>&);
    ~ModuleBox();

    void paint (Graphics&) override;
    void resized() override;
    void moved() override;
    
    // The hosted module
    Module* module;

//==================================================================================
private:

    // Our LookAndFeel instance for this module box
    LookAndFeel_V4 lookandfeel;

    const Colour highlightColour;

    // Layout;
    Rectangle<float> moduleBoxRectangle;
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
    SelectedItemSet<ModuleBox*>& moduleSelection;
    // Variables to manage its own selection
    bool selectResult;
    bool isSelected;

    //==================================================================================

    void setupLookAndFeel ();

    // Dragger wrappers for selected items
    void startDraggingSelected(const MouseEvent&);
    void dragSelected(const MouseEvent&);

    // Button Listeners
    void buttonStateChanged (Button*) override;
    void buttonClicked (Button*) override {};

    void mouseDown(const MouseEvent&) override;
    void mouseUp(const MouseEvent&) override;
    void mouseDrag(const MouseEvent&) override;

    void changeListenerCallback (ChangeBroadcaster*) override;

    //void buttonStateChanged (Button*) override {}

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ModuleBox)
};
