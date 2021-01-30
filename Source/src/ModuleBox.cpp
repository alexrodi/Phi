/*
  ==============================================================================

    ModuleBox.cpp
    Created: 8 Feb 2020 12:28:45pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

///@cond
#include <JuceHeader.h>
///@endcond
#include "ModuleBox.h"

//==============================================================================
ModuleBox::ModuleBox(std::unique_ptr<ModuleUI> module, SelectedItemSet<ModuleBox*>& selectionChangeSource) :
moduleUI{std::move(module)},
highlightColour{Colours::cyan.withSaturation(0.5f)},
powerButton{},
resizer(this, this),
moduleSelection{selectionChangeSource}
{
    powerButton.setToggleState(true, NotificationType());
    
    // Listeners ======================================================
    powerButton.addListener(this);
    moduleSelection.addChangeListener(this);
    
    // Sizes ======================================================
    // Box size constraints
    setSizeLimits(100, HEADER_HEIGHT + 5, 500, 300);
    resizer.setSize(8,8);
    
    // Visibles ======================================================
    addAndMakeVisible(*moduleUI);
    addAndMakeVisible(powerButton);
    addAndMakeVisible(resizer);
    setPaintingIsUnclipped(true);
    setBufferedToImage(true);
    setBroughtToFrontOnMouseClick(true);
    
    setupLookAndFeel();
    
    setSize(moduleUI->props.width, moduleUI->props.height);
}

ModuleBox::~ModuleBox()
{
    moduleSelection.removeChangeListener(this);
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
    g.drawText(moduleUI->props.name, nameRectangle, Justification::centredLeft, false); // (uses color from outline)
    
    // Header Line
    g.setColour (Colours::grey);
    g.fillRect(headerLine);
}

void ModuleBox::resized()
{
    // Check height to constrain size
    if (getHeight() < moduleUI->props.minimumHeight)
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
    auto moduleRect = getLocalBounds();
    // Header area
    auto boxHeader = moduleRect.removeFromTop(HEADER_HEIGHT);
    
    // Place Power button
    powerButton.setBounds(boxHeader.removeFromLeft(35).reduced(CONTENT_PADDING,6));
    
    // Place Text
    nameRectangle = boxHeader.toFloat();
    
    // Place Module
    moduleUI->setBounds(moduleRect.reduced(CONTENT_PADDING));// (padded)
    
    
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
    lookandfeel.setColour(Slider::rotarySliderOutlineColourId, Colour::greyLevel(0.21));
    lookandfeel.setColour(Slider::rotarySliderFillColourId, Colour::greyLevel(0.17));
    lookandfeel.setColour(Slider::textBoxHighlightColourId, Colour::greyLevel(0.2));
    lookandfeel.setColour(Slider::textBoxTextColourId, Colours::grey.brighter());
    lookandfeel.setColour(Slider::textBoxOutlineColourId, Colour()); // no color
    lookandfeel.setColour(Label::backgroundWhenEditingColourId, Colour::greyLevel(0.3));
    lookandfeel.setColour(CaretComponent::caretColourId, Colour::greyLevel(0.8));
    lookandfeel.setColour(TextEditor::focusedOutlineColourId, Colour());
    lookandfeel.setColour(TextEditor::highlightedTextColourId, Colour::greyLevel(0.7));
    lookandfeel.setColour(TextButton::textColourOnId, Colours::grey.brighter());
    
    sendLookAndFeelChange();
}


//==============================================================================

void ModuleBox::forEachSelected(std::function<void(ModuleBox*, const MouseEvent&)> callback, const MouseEvent& e)
{
    auto selected = moduleSelection.getItemArray();
    for (auto& module : selected)
       callback(module, e);
}

void ModuleBox::mouseDown(const MouseEvent& e)
{
    selectionResult = moduleSelection.addToSelectionOnMouseDown(this,ModifierKeys::getCurrentModifiers());
    
    forEachSelected(startDraggingModule, e);
}

void ModuleBox::mouseUp(const MouseEvent& e)
{
    moduleSelection.addToSelectionOnMouseUp(this,ModifierKeys::getCurrentModifiers(),e.mouseWasDraggedSinceMouseDown(), selectionResult);
}

void ModuleBox::mouseDrag(const MouseEvent& e)
{
    forEachSelected(dragModule, e);
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


void ModuleBox::buttonClicked (Button* button)
{
    if (button == &powerButton){
        
        LookAndFeel& lookAndFeel = getLookAndFeel();
        
        if (button->getToggleState())
        {
            lookAndFeel.setColour(Slider::thumbColourId, highlightColour);
            lookAndFeel.setColour(Slider::textBoxTextColourId, Colours::grey.brighter());
            lookAndFeel.setColour(TextButton::textColourOnId, Colours::grey.brighter());
        }
        else
        {
            lookAndFeel.setColour(Slider::thumbColourId, Colours::grey);
            lookAndFeel.setColour(Slider::textBoxTextColourId, Colours::grey);
            lookAndFeel.setColour(TextButton::textColourOnId, Colours::grey);
        }
        
        sendLookAndFeelChange();
    }
        
}
