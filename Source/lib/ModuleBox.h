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
#include "UIobjects/ui_ToggleButton.h"

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
    const float HEADER_HEIGHT = 27;
    const float CONTENT_PADDING = 10;

    // Our LookAndFeel instance for this module box
    LookAndFeel_V4 lookandfeel;

    const Colour highlightColour;

    // Layout;
    Rectangle<float> moduleBoxRectangle;
    Rectangle<float> headerLine;
    Rectangle<float> nameRectangle;

    //==================================================================================
    ui_ToggleButton powerButton;


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
