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
ModuleBox::ModuleBox(SelectedItemSet<ModuleBox*>* selectionChangeSource) :
module{},
contentPadding(10),
headerHeight(27),
powerButton{},
resizer(this, this),
moduleSelection{selectionChangeSource}
{
    // Listeners ======================================================
    powerButton.addListener(this);
    
    moduleSelection->addChangeListener(this);
    
    // Sizes ======================================================
    // Box size constraints are static
    setSizeLimits(200,headerHeight+5,500,300);
    resizer.setSize(8,8);
    
    // Visibles ======================================================
    addAndMakeVisible(module);
    addAndMakeVisible(powerButton);
    addAndMakeVisible(resizer);
    setPaintingIsUnclipped(true);
    setBroughtToFrontOnMouseClick(true);
    
    // LookAndFeel ======================================================
    setLookAndFeel(&lookandfeel);
    
    lookandfeel.setColour(phi_Dial::thumbColourId, Colours::cyan.brighter());
    lookandfeel.setColour(phi_Dial::rotarySliderFillColourId, Colour::greyLevel(0.17));
    lookandfeel.setColour(phi_Dial::rotarySliderOutlineColourId, Colour::greyLevel(0.2));
    lookandfeel.setColour(phi_Dial::textBoxOutlineColourId, Colour()); // no color
    lookandfeel.setColour(phi_Dial::textBoxHighlightColourId, Colour::greyLevel(0.2));
    lookandfeel.setColour(TextEditor::focusedOutlineColourId, Colour());
    lookandfeel.setColour(TextEditor::highlightedTextColourId, Colour::greyLevel(0.7));
    lookandfeel.setColour(Label::backgroundWhenEditingColourId, Colour::greyLevel(0.3));
    lookandfeel.setColour(CaretComponent::caretColourId, Colour::greyLevel(0.8));
    
    sendLookAndFeelChange();
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
    g.fillRoundedRectangle(moduleRectangle, 2.f);
    
    // Outline
    g.setColour (isSelected ? Colours::grey.brighter() : Colours::grey);
    g.drawRoundedRectangle(moduleRectangle, 2.f, isSelected ? 2 : 0.5);
    
    // Module Name
    g.drawText(module.moduleName, nameRectangle, Justification::centredLeft, false); // (uses color from outline)
    
    // Header Line
    g.setColour (Colours::grey);
    g.fillRect(headerLine);
}

void ModuleBox::resized()
{
    // Check height to constrain size
    if (getHeight() < 100)
        setBounds(getX(), getY(), getWidth(), 30);
    
    // Module Box area (padded)
    moduleRectangle = getLocalBounds().toFloat().reduced(1.5, 1.5);
    
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
    headerLine = Rectangle<float>(contentPadding, headerHeight-2, getWidth()-contentPadding*2, 1);
    
    // Module area
    Rectangle<int> moduleRect = getLocalBounds();
    // Header area
    Rectangle<int> boxHeader = moduleRect.removeFromTop(headerHeight);
    
    // Place Power button
    powerButton.setBounds(boxHeader.removeFromLeft(35).reduced(contentPadding,6));
    
    // Place Text
    nameRectangle = Rectangle<float>(boxHeader.toFloat());
    
    // Place Module
    module.setBounds(moduleRect.reduced(contentPadding));// (padded)
}


//==============================================================================
void ModuleBox::startDraggingSelected(const MouseEvent& e){
    Array<ModuleBox*> selected = moduleSelection->getItemArray();
    for (ModuleBox* module : selected){
        module->startDraggingComponent(module, e);
    }
}

void ModuleBox::dragSelected(const MouseEvent& e){
    Array<ModuleBox*> selected = moduleSelection->getItemArray();
    for (ModuleBox* module : selected){
         module->dragComponent(module, e, nullptr);
    }
}

void ModuleBox::mouseDown(const MouseEvent& e)
{
    selectResult = moduleSelection->addToSelectionOnMouseDown(this,ModifierKeys::getCurrentModifiers());
    startDraggingSelected(e);
}

void ModuleBox::mouseUp(const MouseEvent& e)
{
    moduleSelection->addToSelectionOnMouseUp(this,ModifierKeys::getCurrentModifiers(),e.mouseWasDraggedSinceMouseDown(), selectResult);
}

void ModuleBox::mouseDrag(const MouseEvent& e)
{
    dragSelected(e);
    checkComponentBounds(this);
}


//==============================================================================
void ModuleBox::changeListenerCallback (ChangeBroadcaster* source)
{
    if (source == moduleSelection)
    {
        /// here we get a change from the selected modules list
        /// so the module must update its UI accordingly
        isSelected = moduleSelection->isSelected(this);
        repaint();
    }
}


void ModuleBox::buttonStateChanged (Button* button)
{
    if (button == &powerButton){
        
        LookAndFeel& lookAndFeel = getLookAndFeel();
        
        if (button->getToggleState())
            lookAndFeel.setColour(Slider::thumbColourId, Colours::cyan.brighter());
        else
            lookAndFeel.setColour(Slider::thumbColourId, Colours::grey);
        
        sendLookAndFeelChange();
    }
        
}
