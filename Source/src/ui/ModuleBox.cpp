/*
  ==============================================================================

    ModuleBox.cpp
    Created: 8 Feb 2020 12:28:45pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#include "ModuleBox.h"

//==============================================================================
ModuleBox::ModuleBox(State& state, ModuleID moduleID, std::unique_ptr<ModuleUI> moduleUi) :
state(state),
moduleUI{std::move(moduleUi)},
resizer(this, this),
moduleID(moduleID)
{
    state.addListener(this);
    
    powerButton.setToggleState(true, juce::dontSendNotification);
    
    powerButton.onClick = [&, moduleID] () {
        state.setModuleEnabled(moduleID, powerButton.getToggleState());
    };
    
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
    
    setSize(moduleUI->props.defaultSize.width, moduleUI->props.defaultSize.height);
}

ModuleBox::~ModuleBox()
{
    setLookAndFeel (nullptr);
    state.removeListener(this);
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
    int collapsedHeight = headerHeight;
    bool isPortsOnly = getWidth() < moduleUI->props.minimumSize.width;
    int portsOnlyMinimumHeight = headerHeight + (int)std::max(inlets.size(), outlets.size()) * 30;
    bool isCollapsed = getHeight() < (isPortsOnly ? portsOnlyMinimumHeight : moduleUI->props.minimumSize.height);
    
    int portsOnlyWidth = 0;
    if (!inlets.empty()) portsOnlyWidth += portColumnWidth;
    if (!outlets.empty()) portsOnlyWidth += portColumnWidth;
    
    setSizeLimits(
        isCollapsed ? 100 : portsOnlyWidth,
        collapsedHeight,
        500,
        300
    );
    
    if (isCollapsed)
        setSize(getWidth(), collapsedHeight);
    else if (isPortsOnly)
        setSize(portsOnlyWidth, getHeight());
        
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
    resizer.setBounds(getLocalBounds().reduced(3).removeFromBottom(8).removeFromRight(8));
    
    // Place header line
    if (isCollapsed) headerLine = {};
    else headerLine = {padding, headerHeight - 2.0f, getWidth()-padding * 2.0f, 1.0f};
    
    // Module area
    auto moduleRect = getLocalBounds();
    // Header area
    auto boxHeader = moduleRect.removeFromTop(headerHeight);
    
    // Place Power button
    powerButton.setBounds(boxHeader.removeFromLeft(35).reduced(padding, 6));
    
    // Place Text
    nameRectangle = boxHeader.toFloat();
    
    if (isCollapsed)
        moduleRect = getLocalBounds().withSizeKeepingCentre(getWidth() + portColumnWidth + 2, 0);
    
    // Place Ports
    moduleRect = placeInletsAndOutlets(moduleRect);
    
    // Place Module
    moduleUI->setVisible(!isCollapsed);
    moduleUI->setBounds(moduleRect.reduced(0, padding));
    
    state.setModuleBounds(moduleID, getBounds());
}

void ModuleBox::moved() {
    state.setModuleBounds(moduleID, getBounds());
}

void ModuleBox::setHighlightColour(const juce::Colour& colour)
{
    lookandfeel.setHighlightColour(colour);
    lookandfeel.setModuleOn(powerButton.getToggleState());
    sendLookAndFeelChange();
}

void ModuleBox::showPortLabelsChanged(ShowPortLabels show) {
    for (auto& port : inlets)
        port->showLabel(show);
    
    for (auto& port : outlets)
        port->showLabel(show);
}

void ModuleBox::moduleEnabledChanged(ModuleID id, bool isEnabled) {
    if (id == moduleID) {
        powerButton.setToggleState(isEnabled, juce::dontSendNotification);
        lookandfeel.setModuleOn(isEnabled);
        sendLookAndFeelChange();
    }
}

PortID ModuleBox::getPortID(const PortUI& port) const {
    auto& v = port.getType() == PortType::Inlet ? inlets : outlets;
    
    auto it = std::find_if(v.begin(), v.end(), [&] (auto& item) { return item.get() == &port; });

    if (it == v.end()) return -1;
    
    return (PortID)std::distance(v.begin(), it);
}

const PortUI& ModuleBox::getPort(PortType type, PortID portID) const {
    if (type == PortType::Inlet) return *inlets[portID];
    else return *outlets[portID];
}

void ModuleBox::placePorts(const std::vector<std::unique_ptr<PortUI>>& ports, juce::Rectangle<int> bounds)
{
    const int portHeight = bounds.getHeight() / (int)ports.size();
    
    // Divide the space for each Port
    for (auto& port : ports)
        port->setBounds( bounds.removeFromTop(portHeight) );
}

juce::Rectangle<int> ModuleBox::placeInletsAndOutlets(juce::Rectangle<int> bounds)
{
    if (!inlets.empty())
        placePorts(inlets, bounds.removeFromLeft(portColumnWidth));
    
    if (!outlets.empty())
        placePorts(outlets, bounds.removeFromRight(portColumnWidth));
    
    return bounds;
}
