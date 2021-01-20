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
    ModuleBox(std::unique_ptr<ModuleUI>, SelectedItemSet<ModuleBox*>&);
    ~ModuleBox();

    void paint (Graphics&) override;
    void resized() override;
    void moved() override;
    
    /// The hosted moduleUI
    std::unique_ptr<ModuleUI> moduleUI;

//==================================================================================
private:
    const float HEADER_HEIGHT = 27;
    const float CONTENT_PADDING = 10;

    /// Our LookAndFeel instance for this module box
    LookAndFeel_V4 lookandfeel;
    /** The highlight colour reflected in the power button
    This propagates to all the child components and sets UI component colours,
     reflecting the enabled state of the ModuleBox */
    const Colour highlightColour;

    /// This box's rectangle
    Rectangle<float> moduleBoxRectangle;
    /// The line dividing the header from the module
    Rectangle<float> headerLine;
    /// The module name's rectangle
    Rectangle<float> nameRectangle;

    //==================================================================================
    
    /// The top-left button for enabling and disabling the contained module
    ui_ToggleButton powerButton;

    /// Imposes a draggable corner on the component for resizing
    ResizableCornerComponent resizer;

    /// Pointer to the MainPatcher's selection set
    SelectedItemSet<ModuleBox*>& moduleSelection;
    
    /** Used to store mouseDown selection
     @see SelectedItemSet::addToSelectionOnMouseDown, SelectedItemSet::addToSelectionOnMouseUp
    */
    bool selectionResult;
    /// Used to store the selected state
    bool isSelected;

    //==================================================================================
    
    /// Defines all the colors that must conform to this module box, the modified LookAndFeel passes to the child components and they use these colors
    void setupLookAndFeel ();

    /// Wrapper for startDraggingComponent()
    static void startDraggingModule(ModuleBox* module, const MouseEvent& e)
    {
        module->startDraggingComponent(module, e);
    }
    /// Wrapper for dragComponent()
    static void dragModule(ModuleBox* module, const MouseEvent& e)
    {
        module->dragComponent(module, e, module);
    }
    /// Runs a drag functtion (dragModule() orstartDraggingComponent()) on every currently selected ModuleBox
    void forEachSelected(std::function<void(ModuleBox*, const MouseEvent&)>, const MouseEvent&);

    /// Callback for Buttons (e.g. power button)
    void buttonClicked (Button*) override;

    void mouseDown(const MouseEvent&) override;
    void mouseUp(const MouseEvent&) override;
    void mouseDrag(const MouseEvent&) override;

    /// Currently only used to get notitiffed when the module selection changes (draws a border around the module when selected))
    void changeListenerCallback (ChangeBroadcaster*) override;
    
    ///@cond
    void buttonStateChanged (Button*) override {};
    ///@endcond

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ModuleBox)
};
