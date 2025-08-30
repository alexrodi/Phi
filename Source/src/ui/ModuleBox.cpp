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
resizer(this, nullptr),
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
    setLookAndFeel(nullptr);
    state.removeListener(this);
}

//==============================================================================
void ModuleBox::paint (juce::Graphics& g)
{
    drawBox(g);
    
    // Module Name
    g.setColour (findColour(isSelected ? PhiColourIds::Module::SelectedName : PhiColourIds::Module::Name));
    g.drawText(moduleUI->props.name, nameRectangle, juce::Justification::centredLeft, false); // (uses color from outline)
    
    // Header Line
    g.setColour (findColour(PhiColourIds::Module::Name));
    g.fillRect(headerLine);
}

juce::Path ModuleBox::getCollapsedBox() {
    juce::Path path;
    
    auto r45 = roundness * 0.45f;
    auto x = boxBounds.getX();
    auto y = boxBounds.getY();
    auto x2 = boxBounds.getRight();
    auto y2 = boxBounds.getBottom();
    auto yc = boxBounds.getCentreY();
    auto radius = 4.0f;
    
    // Top Left
    path.startNewSubPath (x, y + roundness);
    path.cubicTo (x, y + r45, x + r45, y, x + roundness, y);
    
    // Top Right
    path.lineTo (x2 - roundness, y);
    path.cubicTo (x2 - r45, y, x2, y + r45, x2, y + roundness);
    
    // Outlet Arc
    if (numOutletsConnected > 0)
        path.addCentredArc(x2, yc, radius, radius, 0.0f, juce::MathConstants<float>::twoPi, juce::MathConstants<float>::pi);
    
    // Bottom Right
    path.lineTo (x2, y2 - roundness);
    path.cubicTo (x2, y2 - r45, x2 - r45, y2, x2 - roundness, y2);
    
    // Bottom Left
    path.lineTo (x + roundness, y2);
    path.cubicTo (x + r45, y2, x, y2 - r45, x, y2 - roundness);
    
    // Inlet Arc
    if (numInletsConnected > 0)
        path.addCentredArc(x, yc, radius, radius, 0.0f, juce::MathConstants<float>::pi, 0.0f);
    
    path.closeSubPath();
    return path;
}

void ModuleBox::drawBox(juce::Graphics& g) {
    juce::Path path;
    
    if (isCollapsed) path = getCollapsedBox();
    else path.addRoundedRectangle(boxBounds, roundness);
    
    float stroke = isSelected ? selectedOutlineThickness : outlineThickness;
    g.setColour(findColour(isSelected ? PhiColourIds::Module::SelectedOutline : PhiColourIds::Module::Outline));
    g.strokePath(path, juce::PathStrokeType{stroke});
    
    g.setColour(findColour(PhiColourIds::Module::Background));
    g.fillPath(path);
}

void ModuleBox::handleCollapse() {
    int portsOnlyWidth = 0;
    if (!inlets.empty()) portsOnlyWidth += portColumnWidth;
    if (!outlets.empty()) portsOnlyWidth += portColumnWidth;
    
    auto minimum = moduleUI->props.minimumSize;
    int width = getWidth();
    
    if (getHeight() < minimum.height)
    {
        if (getHeight() < minimum.height / 2)
            setSize(width, headerHeight); // Collapse height to show header only
        else
            setSize(width, minimum.height);
    }
    
    isCollapsed = getHeight() == headerHeight;
    
    if (!isCollapsed && width < minimum.width)
    {
        if (width < minimum.width / 2)
            setSize(portsOnlyWidth, getHeight()); // Collapse width to show ports only
        else
            setSize(minimum.width, getHeight());
    }
}

void ModuleBox::enforceSizeLimits() {
    int minHeight = headerHeight;
    
    // Make sure there's width for the power button and module name
    int minWidth = padding * 3 + powerButtonSize;
    minWidth += juce::GlyphArrangement::getStringWidth(lookandfeel.withDefaultMetrics({}), moduleUI->props.name);
    minWidth += padding; // Plus some right padding
    
    auto maxSize = moduleUI->props.minimumSize;
    maxSize.width *= 2;
    maxSize.height *= 2;
    
    setSize(
        clip(getWidth(), minWidth, maxSize.width),
        clip(getHeight(), minHeight, maxSize.height)
    );
}

void ModuleBox::resized()
{
    if (resizeIsReentrant) return;
    
    {
        // We do a lot of setSize() here so this avoids needless recursive calls
        const juce::ScopedValueSetter<bool> svs (resizeIsReentrant, true);
        
        enforceSizeLimits();
        handleCollapse();
    }
        
    // Module Box area (padded)
    boxBounds = getLocalBounds().toFloat().reduced(1.5f, 1.5f);
    
    // Place resizer in bottom right corner
    resizer.setBounds(getLocalBounds().reduced(3).removeFromBottom(8).removeFromRight(8));
    
    // Place header line
    if (!isCollapsed)
        headerLine = {padding, headerHeight - 2.0f, getWidth()-padding * 2.0f, 1.0f};
    else headerLine = {};
    
    // Module area
    auto moduleRect = getLocalBounds();
    // Header area
    auto header = moduleRect.removeFromTop(headerHeight);
    
    // Place Power button
    powerButton.setBounds(header.removeFromLeft(padding * 2 + powerButtonSize)
        .withSizeKeepingCentre(powerButtonSize, powerButtonSize));
    
    // Place Text
    nameRectangle = header.toFloat();
    
    // Place Ports & ModuleUI
    moduleRect = placeInletsAndOutlets(moduleRect);
    moduleUI->setBounds(moduleRect.reduced(0, padding));
    moduleUI->setVisible(!isCollapsed);
    
    // Inform state of any updates to the module bounds
    state.setModuleBounds(moduleID, getBounds());
}

void ModuleBox::moved() {
    state.setModuleBounds(moduleID, getBounds());
}

void ModuleBox::colourChanged() {
    setTheme();
}

void ModuleBox::parentHierarchyChanged() {
    setTheme();
}

void ModuleBox::setTheme() {
    if (auto* parent = getParentComponent())
    {
        if (auto* mainLookandFeel = static_cast<PhiLookAndFeel*>(&parent->getLookAndFeel()))
        {
            lookandfeel.setTheme(mainLookandFeel->getTheme());
            moduleUI->sendLookAndFeelChange();
        }
    }
}

PortID ModuleBox::getPortID(const PortUI& port) const {
    auto& v = port.getType() == PortType::Inlet ? inlets : outlets;
    
    auto it = std::find_if(v.begin(), v.end(), [&] (auto& item) { return item.get() == &port; });

    if (it == v.end()) return -1;
    
    return (int)std::distance(v.begin(), it);
}

const PortUI& ModuleBox::getPort(PortType type, PortID portID) const {
    if (type == PortType::Inlet) return *inlets[portID];
    else return *outlets[portID];
}

void ModuleBox::placePorts(const std::vector<std::unique_ptr<PortUI>>& ports, juce::Rectangle<int> bounds)
{
    const int portHeight = bounds.getHeight() / (int)ports.size();
    
    for (auto& port : ports)
        port->setBounds( bounds.removeFromTop(portHeight) );
}

juce::Rectangle<int> ModuleBox::placeInletsAndOutlets(juce::Rectangle<int> bounds)
{
    // If box is collapsed we vertically center the ports and set a height of 0
    if (isCollapsed)
        bounds = getLocalBounds().withSizeKeepingCentre(getWidth() + portColumnWidth - 2, 0);
    
    if (!inlets.empty())
        placePorts(inlets, bounds.removeFromLeft(portColumnWidth));
    
    if (!outlets.empty())
        placePorts(outlets, bounds.removeFromRight(portColumnWidth));
    
    return bounds;
}

void ModuleBox::setShowPortLabels(ShowPortLabels show) {
    int newPortColumnWidth = show == ShowPortLabels::On ? 50 : 36;
    
    if (portColumnWidth != newPortColumnWidth) {
        portColumnWidth = newPortColumnWidth;
        resized();
    }
    
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

void ModuleBox::moduleColourChanged(ModuleID id, const juce::Colour& colour) {
    if (id == moduleID) {
        lookandfeel.setCustomHighlightColour(colour);
        sendLookAndFeelChange();
    }
}

void ModuleBox::connectionCreated(ConnectionID id) {
    if (id.source.moduleID == moduleID) {
        numOutletsConnected++;
        repaint();
    } else if (id.destination.moduleID == moduleID) {
        numInletsConnected++;
        repaint();
    }
}

void ModuleBox::connectionDeleted(ConnectionID id) {
    if (id.source.moduleID == moduleID) {
        numOutletsConnected--;
        repaint();
    } else if (id.destination.moduleID == moduleID) {
        numInletsConnected--;
        repaint();
    }
}
