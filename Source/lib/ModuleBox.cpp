/*
  ==============================================================================

    ModuleBox.cpp
    Created: 8 Feb 2020 12:28:45pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ModuleBox.h"

//==============================================================================
ModuleBox::ModuleBox(Module* module, SelectedItemSet<ModuleBox*>& selectionChangeSource) :
module{module},
highlightColour{Colours::cyan.brighter()},
powerButton{},
resizer(this, this),
moduleSelection{selectionChangeSource}
{
    // Listeners ======================================================
    powerButton.addListener(this);
    
    moduleSelection.addChangeListener(this);
    
    // Sizes ======================================================
    // Box size constraints
    setSizeLimits(100, HEADER_HEIGHT + 5, 500, 300);
    resizer.setSize(8,8);
    
    // Visibles ======================================================
    addAndMakeVisible(*module);
    addAndMakeVisible(powerButton);
    addAndMakeVisible(resizer);
    setPaintingIsUnclipped(true);
    setBroughtToFrontOnMouseClick(true);
    
    setupLookAndFeel();
    
    setSize(module->props.width, module->props.height);
}

ModuleBox::~ModuleBox()
{
    setLookAndFeel (nullptr);
}

//==============================================================================
void ModuleBox::paint (Graphics& g)
{
    // Box
    g.setColour (Colours::darkgrey.darker());
    g.fillRoundedRectangle(moduleBoxRectangle, 2.f);
    
    // Outline
    g.setColour (isSelected ? Colours::grey.brighter() : Colours::grey);
    g.drawRoundedRectangle(moduleBoxRectangle, 2.f, isSelected ? 2 : 0.5);
    
    // Module Name
    g.drawText(module->props.name, nameRectangle, Justification::centredLeft, false); // (uses color from outline)
    
    // Header Line
    g.setColour (Colours::grey);
    g.fillRect(headerLine);
}

void ModuleBox::resized()
{
    // Check height to constrain size
    if (getHeight() < module->props.minimumHeight)
        setSize(getWidth(), HEADER_HEIGHT + 3);
    
    // Module Box area (padded)
    moduleBoxRectangle = getLocalBounds().toFloat().reduced(1.5, 1.5);
    
    // Set box position constraints
    /**
     setMinimumOnscreenAmounts assures the full module should be on screen,
        although it still makes it possible for modules to be off screen when the window size changes,
           should learn more about this behaviour, we don't want to open a patch and have everything jumbled in a small window
     */
    setMinimumOnscreenAmounts(getHeight(), getWidth(), getHeight(), getWidth());
    
    // Place resizer in bottom left corner
    resizer.setBoundsToFit(getLocalBounds().reduced(3), Justification::bottomRight, true);
    
    // Place header line
    headerLine = Rectangle<float>(CONTENT_PADDING, HEADER_HEIGHT - 2, getWidth()-CONTENT_PADDING * 2, 1);
    
    // Module area
    Rectangle<int> moduleRect = getLocalBounds();
    // Header area
    Rectangle<int> boxHeader = moduleRect.removeFromTop(HEADER_HEIGHT);
    
    // Place Power button
    powerButton.setBounds(boxHeader.removeFromLeft(35).reduced(CONTENT_PADDING,6));
    
    // Place Text
    nameRectangle = Rectangle<float>(boxHeader.toFloat());
    
    // Place Module
    module->setBounds(moduleRect.reduced(CONTENT_PADDING));// (padded)
    
    
    // Emmit action message to update connections
    sendActionMessage("moduleChanged");
}

void ModuleBox::moved()
{
    // Emmit action message to update connections
    sendActionMessage("moduleChanged");
}

void ModuleBox::setupLookAndFeel()
{
    setLookAndFeel(&lookandfeel);
    
    lookandfeel.setColour(Slider::thumbColourId, highlightColour);
    lookandfeel.setColour(Slider::rotarySliderFillColourId, Colour::greyLevel(0.17));
    lookandfeel.setColour(Slider::rotarySliderOutlineColourId, Colour::greyLevel(0.2));
    lookandfeel.setColour(Slider::textBoxOutlineColourId, Colour()); // no color
    lookandfeel.setColour(Slider::textBoxHighlightColourId, Colour::greyLevel(0.2));
    lookandfeel.setColour(TextEditor::focusedOutlineColourId, Colour());
    lookandfeel.setColour(TextEditor::highlightedTextColourId, Colour::greyLevel(0.7));
    lookandfeel.setColour(Label::backgroundWhenEditingColourId, Colour::greyLevel(0.3));
    lookandfeel.setColour(CaretComponent::caretColourId, Colour::greyLevel(0.8));
    
    sendLookAndFeelChange();
}


//==============================================================================
void ModuleBox::startDraggingSelected(const MouseEvent& e){
    Array<ModuleBox*> selected = moduleSelection.getItemArray();
    for (ModuleBox* module : selected){
        module->startDraggingComponent(module, e);
    }
}

void ModuleBox::dragSelected(const MouseEvent& e){
    Array<ModuleBox*> selected = moduleSelection.getItemArray();
    for (ModuleBox* module : selected){
         module->dragComponent(module, e, module);
    }
}

void ModuleBox::mouseDown(const MouseEvent& e)
{
    selectResult = moduleSelection.addToSelectionOnMouseDown(this,ModifierKeys::getCurrentModifiers());
    startDraggingSelected(e);
}

void ModuleBox::mouseUp(const MouseEvent& e)
{
    moduleSelection.addToSelectionOnMouseUp(this,ModifierKeys::getCurrentModifiers(),e.mouseWasDraggedSinceMouseDown(), selectResult);
}

void ModuleBox::mouseDrag(const MouseEvent& e)
{
    dragSelected(e);
}


//==============================================================================
void ModuleBox::changeListenerCallback (ChangeBroadcaster* source)
{
    if (source == &moduleSelection)
    {
        /// here we get a change from the selected modules list
        /// so the module must update its UI accordingly
        bool updatedIsSelected = moduleSelection.isSelected(this);
        if (updatedIsSelected != isSelected)
        {
            isSelected = updatedIsSelected;
            repaint();
        }
    }
}


void ModuleBox::buttonStateChanged (Button* button)
{
    if (button == &powerButton){
        
        LookAndFeel& lookAndFeel = getLookAndFeel();
        
        if (button->getToggleState())
            lookAndFeel.setColour(Slider::thumbColourId, highlightColour);
        else
            lookAndFeel.setColour(Slider::thumbColourId, Colours::grey);
        
        sendLookAndFeelChange();
    }
        
}
