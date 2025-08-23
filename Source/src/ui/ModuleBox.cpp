/*
  ==============================================================================

    ModuleBox.cpp
    Created: 8 Feb 2020 12:28:45pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#include "ModuleBox.h"

//==============================================================================
ModuleBox::ModuleBox(std::unique_ptr<ModuleUI> moduleUi) :
moduleUI{std::move(moduleUi)},
resizer(this, this)
{
    powerButton.setToggleState(true, juce::sendNotification);
    
    powerButton.addListener(this);
    
    // Box size constraints
    setSizeLimits(100, HEADER_HEIGHT + 5, 500, 300);
    resizer.setSize(8,8);
    
    addAndMakeVisible(*moduleUI);
    addAndMakeVisible(powerButton);
    
    for (auto& inletName : moduleUI->props.inlets) {
        inlets.push_back(std::make_unique<InletUI>(inletName));
        addAndMakeVisible(*inlets.back());
    }
    
    for (auto& outletName : moduleUI->props.outlets) {
        outlets.push_back(std::make_unique<OutletUI>(outletName));
        addAndMakeVisible(*outlets.back());
    }
    
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
    setLookAndFeel (nullptr);
}

//==============================================================================
void ModuleBox::paint (juce::Graphics& g)
{
    // Box
    g.setColour (findColour(PhiColourIds::Module::Background));
    g.fillRoundedRectangle(moduleBoxRectangle, 2.0f);
    
    // Outline
    g.setColour (findColour(isSelected ? PhiColourIds::Module::SelectedOutline : PhiColourIds::Module::Outline));
    g.drawRoundedRectangle(moduleBoxRectangle, 2.0f, isSelected ? 2.0f : 0.5f);
    
    // Module Name
    g.setColour (findColour(isSelected ? PhiColourIds::Module::SelectedText : PhiColourIds::Module::Text));
    g.drawText(moduleUI->props.name, nameRectangle, juce::Justification::centredLeft, false); // (uses color from outline)
    
    // Header Line
    g.setColour (findColour(PhiColourIds::Module::HeaderLine));
    g.fillRect(headerLine);
}

void ModuleBox::resized()
{
    // Check height to constrain size
    bool isCollapsed = getHeight() < moduleUI->props.minimumHeight;
    if (isCollapsed)
        setSize(getWidth(), HEADER_HEIGHT);
    
    // Module Box area (padded)
    moduleBoxRectangle = getLocalBounds().toFloat().reduced(1.5f, 1.5f);
    
    // Set box position constraints
    /**
     setMinimumOnscreenAmounts assures the full module should be on screen,
        although it still makes it possible for modules to be off screen when the window size changes,
           should learn more about this behaviour, we don't want to open a patch and have everything jumbled in a small window
     */
    setMinimumOnscreenAmounts(getHeight(), getWidth(), getHeight(), getWidth());
    
    // Place resizer in bottom right corner
    resizer.setBoundsToFit(getLocalBounds().reduced(3), juce::Justification::bottomRight, true);
    
    // Place header line
    headerLine = {CONTENT_PADDING, HEADER_HEIGHT - 2.0f, getWidth()-CONTENT_PADDING * 2.0f, isCollapsed ? 0.0f : 1.0f};
    
    // Module area
    auto moduleRect = getLocalBounds();
    // Header area
    auto boxHeader = moduleRect.removeFromTop(HEADER_HEIGHT);
    
    // Place Power button
    powerButton.setBounds(boxHeader.removeFromLeft(35).reduced(CONTENT_PADDING, 6));
    
    // Place Text
    nameRectangle = boxHeader.toFloat();
    
    // Place Ports
    moduleRect = placeInletsAndOutlets(moduleRect);
    
    // Place Module
    moduleUI->setVisible(!isCollapsed);
    moduleUI->setBounds(moduleRect.reduced(0, CONTENT_PADDING));
}

void ModuleBox::setHighlightColour(const juce::Colour& colour)
{
    lookandfeel.setHighlightColour(colour);
    lookandfeel.setModuleOn(powerButton.getToggleState());
    sendLookAndFeelChange();
}

void ModuleBox::setShowPortLabels(ShowPortLabels show) {
    for (auto& port : inlets)
        port->showLabel(show);
    
    for (auto& port : outlets)
        port->showLabel(show);
}

void ModuleBox::buttonClicked (juce::Button* button)
{
    if (button == &powerButton){
        auto state = button->getToggleState();
        
        moduleUI->props.processor.suspendProcessing(!state);
        lookandfeel.setModuleOn(state);
        sendLookAndFeelChange();
    }
        
}

int ModuleBox::getPortIndex(const PortUI& port) const {
    auto& v = port.getType() == PortType::Inlet ? inlets : outlets;
    
    auto it = std::find_if(v.begin(), v.end(), [&] (auto& item) { return item.get() == &port; });

    if (it == v.end()) return -1;
    
    return (int)std::distance(v.begin(), it);
}

void ModuleBox::placePorts(const std::vector<std::unique_ptr<PortUI>>& ports, juce::Rectangle<int> portBounds)
{
    if (ports.empty()) return;
    
    const int portHeight = portBounds.getHeight() / (int)ports.size();
    
    // Divide the space for each Port
    for (auto& port : ports)
        port->setBounds( portBounds.removeFromTop(portHeight) );
}

juce::Rectangle<int> ModuleBox::placeInletsAndOutlets(juce::Rectangle<int> bounds)
{
    placePorts(inlets, bounds.removeFromLeft(PLUG_COLUMN_WIDTH));
    placePorts(outlets, bounds.removeFromRight(PLUG_COLUMN_WIDTH));
    
    return bounds;
}
