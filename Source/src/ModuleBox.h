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

    void paint(Graphics&) override;
    void resized() override;
    void moved() override;
    
    /// The hosted moduleUI
    std::unique_ptr<ModuleUI> moduleUI;
    
    struct ColourIds {
        enum Module {
            Background,
            OutlineAndText,
            SelectedOutlineAndText,
            HeaderLine,
            Highlight
        };
        
        // #TODO define Plug colour ids
        enum Plug {
            IntletOutline,
            IntletCenter,
            OutletCenter,
            OutletOutline,
            Text
        };
    };
    
    /// Sets the highlight colour of the module
    void setHighlightColour(Colour&&);

//==================================================================================
private:
    const float HEADER_HEIGHT = 27;
    const float CONTENT_PADDING = 10;
    
    /// Our LookAndFeel instance for this module box
    struct ModuleLookAndFeel  : public LookAndFeel_V4
    {
        ModuleLookAndFeel()
        {
            setHighlightColour(Colours::cyan.withSaturation(0.5f));
            setColour(ColourIds::Module::Background, Colours::darkgrey.darker());
            setColour(ColourIds::Module::OutlineAndText, Colours::grey);
            setColour(ColourIds::Module::SelectedOutlineAndText, Colours::grey.brighter());
            setColour(ColourIds::Module::HeaderLine, Colours::grey);
            
            setColour(Slider::thumbColourId, findColour(ColourIds::Module::Highlight));
            setColour(Slider::rotarySliderOutlineColourId, Colour::greyLevel(0.21));
            setColour(Slider::rotarySliderFillColourId, Colour::greyLevel(0.17));
            setColour(Slider::textBoxHighlightColourId, Colour::greyLevel(0.2));
            setColour(Slider::textBoxTextColourId, Colours::grey.brighter());
            setColour(Slider::textBoxOutlineColourId, Colour()); // no color
            setColour(Label::backgroundWhenEditingColourId, Colour::greyLevel(0.3));
            setColour(CaretComponent::caretColourId, Colour::greyLevel(0.8));
            setColour(TextEditor::focusedOutlineColourId, Colour());
            setColour(TextEditor::highlightedTextColourId, Colour::greyLevel(0.7));
            setColour(TextButton::textColourOnId, Colours::grey.brighter());
        }
        
        void setModuleOn(bool isOn)
        {
            if (isOn)
            {
                setColour(Slider::thumbColourId, findColour(ColourIds::Module::Highlight));
                setColour(Slider::textBoxTextColourId, Colours::grey.brighter());
                setColour(TextButton::textColourOnId, Colours::grey.brighter());
            }
            else
            {
                setColour(Slider::thumbColourId, Colours::grey);
                setColour(Slider::textBoxTextColourId, Colours::grey);
                setColour(TextButton::textColourOnId, Colours::grey);
            }
        }
        
        /// Sets the highlight colour of the module
        void setHighlightColour(Colour colour)
        {
            setColour(ColourIds::Module::Highlight, colour);
        }
        
    } lookandfeel;

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

    //==================================================================================
    
    /// Defines all the colors that must conform to this module box, the modified LookAndFeel passes to the child components and they use these colors
    void setupLookAndFeel ();

    /// Runs a funtion on every currently selected ModuleBox
    void forEachSelected(std::function<void(ModuleBox*)>);

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
