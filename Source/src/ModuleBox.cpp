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
    
    setLookAndFeel(&lookandfeel);
    sendLookAndFeelChange();
    
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
    g.setColour (findColour(ColourIds::Background));
    g.fillRoundedRectangle(moduleBoxRectangle, 2.f);
    
    bool isSelected = moduleSelection.isSelected(this);
    // Outline
    g.setColour (findColour(isSelected ? ColourIds::SelectedOutlineAndText : ColourIds::OutlineAndText));
    g.drawRoundedRectangle(moduleBoxRectangle, 2.f, isSelected ? 2 : 0.5);
    
    // Module Name
    g.drawText(moduleUI->props.name, nameRectangle, Justification::centredLeft, false); // (uses color from outline)
    
    // Header Line
    g.setColour (findColour(ColourIds::HeaderLine));
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

//==============================================================================

void ModuleBox::forEachSelected(std::function<void(ModuleBox*)> callback)
{
    auto selected = moduleSelection.getItemArray();
    for (auto& module : selected)
       callback(module);
}

void ModuleBox::mouseDown(const MouseEvent& e)
{
    selectionResult = moduleSelection.addToSelectionOnMouseDown(this, e.mods);
    auto selected = moduleSelection.getItemArray();
    for (auto module : selected)
        module->startDraggingComponent(module, e);
}

void ModuleBox::mouseUp(const MouseEvent& e)
{
    moduleSelection.addToSelectionOnMouseUp(this, e.mods, e.mouseWasDraggedSinceMouseDown(), selectionResult);
}

void ModuleBox::mouseDrag(const MouseEvent& e)
{
    forEachSelected([&e](ModuleBox* module){module->dragComponent(module, e, module);});
}


//==============================================================================
void ModuleBox::changeListenerCallback (ChangeBroadcaster* source)
{
    if (source == &moduleSelection) repaint();
}


void ModuleBox::buttonClicked (Button* button)
{
    if (button == &powerButton){
        lookandfeel.setModuleOn(button->getToggleState());
        sendLookAndFeelChange();
    }
        
}
