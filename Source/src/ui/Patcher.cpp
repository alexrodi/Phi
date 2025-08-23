/*
  ==============================================================================

    MainPatcher.cpp
    Created: 29 Mar 2020 12:51:46am
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#include "Patcher.h"
#include "MainComponent.h"

//==============================================================================
Patcher::Patcher(State& state) :
state(state),
connections(state, *this)
{
    state.newModuleUICreated = [&] (auto ui, auto moduleID) {
        auto [it, inserted] = modules.insert({ moduleID, std::make_unique<ModuleBox>(std::move(ui)) });
        
        if (!inserted) return;
        
        ModuleBox& moduleBox = *it->second;
        
        addAndMakeVisible(moduleBox);
    };
    
    setWantsKeyboardFocus(true);
    
    addAndMakeVisible(connections);
    addAndMakeVisible(lasso);
    addChildComponent(tooltip);
    
    selectedModuleIDs.addChangeListener(this);
    state.addListener(this);
    
    setSize(1000, 1000);
    setPaintingIsUnclipped(true);
}

Patcher::~Patcher()
{
    state.removeListener(this);
}

const PortUI* Patcher::getPortUI (ModulePortID portID, PortType type) const {
    if (auto* moduleUI = getModuleUI(portID.moduleID)) {
        if (type == PortType::Inlet)
            return &moduleUI->inlets[portID.portID];
        else
            return &moduleUI->outlets[portID.portID];
    }
    
    return nullptr;
}

ModuleUI* Patcher::getModuleUI (ModuleID moduleIDToFind) const {
    for (auto& [moduleID, item] : modules) {
        if (moduleID == moduleIDToFind)
            return item->moduleUI.get();
    }
    
    return nullptr;
}

std::optional<ModuleID> Patcher::getBoxModuleID(const ModuleBox& box) const {
    for (auto& [moduleID, item] : modules)
        if (item.get() == &box)
            return {moduleID};
    
    return {};
}

std::optional<ModulePortID> Patcher::getPortID(const PortUI& port) const {
    if (auto box = port.findParentComponentOfClass<ModuleBox>()) {
        if (auto moduleID = getBoxModuleID(*box))
            return {{*moduleID, box->moduleUI->getPortIndex(port)}};
    }
    
    return {};
}

//==============================================================================
void Patcher::paint (juce::Graphics& g)
{
}

void Patcher::resized()
{
    connections.setBounds(getLocalBounds());
}

void Patcher::mouseDown(const juce::MouseEvent& e)
{
    if (!e.mods.isShiftDown())
        selectedModuleIDs.deselectAll();
    
    if (e.eventComponent == this)
    {
        if (e.mods.isRightButtonDown())
            openMenu(e);
    }
    else if (auto box = dynamic_cast<ModuleBox*>(e.eventComponent))
    {
        if (auto moduleID = getBoxModuleID(*box)) {
            if (e.mods.isRightButtonDown()) openColourSelector();
            
            selectionResult = selectedModuleIDs.addToSelectionOnMouseDown(*moduleID, e.mods);
            
            forEachSelected([&] (auto moduleID, auto& moduleBox) {
                startDraggingComponent(&moduleBox, e);
            });
        }
    }
}

void Patcher::mouseUp(const juce::MouseEvent& e)
{
    if (e.eventComponent == this) {
        if (e.mouseWasDraggedSinceMouseDown())
            lasso.endLasso();
    }
    else if (auto box = dynamic_cast<ModuleBox*>(e.eventComponent))
    {
        for (auto& [moduleID, item] : modules)
        {
            if (item.get() == box) {
                selectedModuleIDs.addToSelectionOnMouseUp(moduleID, e.mods, e.mouseWasDraggedSinceMouseDown(), selectionResult);
                break;
            }
        }
    }
}

void Patcher::mouseDrag(const juce::MouseEvent& e)
{
    if (e.eventComponent == this) {
        lasso.dragLasso(e);
    }
    else if (auto box = dynamic_cast<ModuleBox*>(e.eventComponent))
    {
        forEachSelected([&] (auto moduleID, auto& moduleBox) {
            dragComponent(&moduleBox, e, &moduleBox);
            state.setModuleBounds(moduleID, moduleBox.getBounds());
        });
    }
}

bool Patcher::keyPressed (const juce::KeyPress& key)
{
    if (key == juce::KeyPress::backspaceKey)
    {
        for (auto moduleID : selectedModuleIDs)
            state.deleteModule(moduleID);
        
        return true;
    }
    return false;
}

void Patcher::openMenu(const juce::MouseEvent& e)
{
    juce::PopupMenu menu;
    menu.addSubMenu ("Add Module...", Modules::getMenu());

    // Returns the ID of the selected item (0 if clicked outside)
    menu.showMenuAsync(juce::PopupMenu::Options().withParentComponent(this), [&, pos = e.position] (int result) {
        if (result > 0)
            state.addModule(Modules::getInfoFromMenuIndex(result-1), pos.x, pos.y);
    });
}

void Patcher::moduleBoundsChanged(ModuleID moduleID, juce::Rectangle<int> bounds) {
    if (!modules.contains(moduleID)) return;
    
    if (bounds.isEmpty())
        modules[moduleID]->setTopLeftPosition(bounds.getTopLeft());
    else
        modules[moduleID]->setBounds(bounds);
}

void Patcher::moduleDeleted(ModuleID moduleID)
{
    modules.erase(moduleID);
}

template<class CallbackType>
void Patcher::forEachSelected(CallbackType callback)
{
    for (auto& moduleID : selectedModuleIDs.getItemArray()) {
        if (modules.contains(moduleID))
            callback(moduleID, *modules[moduleID]);
    }
}

void Patcher::findLassoItemsInArea (juce::Array<ModuleID>& itemsFound, const juce::Rectangle<int>& area)
{
    for (auto& [moduleID, item] : modules) {
        if (item->getBounds().intersects(area))
            itemsFound.add(moduleID);
    }
}

juce::SelectedItemSet<ModuleID>& Patcher::getLassoSelection() {return selectedModuleIDs;}

void Patcher::changeListenerCallback(juce::ChangeBroadcaster* source) {
    if (source == &selectedModuleIDs)
    {
        for (auto& [moduleID, box] : modules)
            box->setSelected(selectedModuleIDs.isSelected(moduleID));
    }
    else if (auto colourSelector = dynamic_cast<juce::ColourSelector*>(source))
    {
        forEachSelected( [&] (auto moduleID, auto& moduleBox) {
            moduleBox.setHighlightColour(colourSelector->getCurrentColour());
        });
    }
}

void Patcher::openColourSelector()
{
    auto colourSelector = std::make_unique<juce::ColourSelector>(juce::ColourSelector::showColourspace);
    colourSelector->setCurrentColour (findColour(PhiColourIds::Module::Highlight));
    colourSelector->setSize (150, 130);
    colourSelector->addChangeListener(this);

    auto& callOutBox = juce::CallOutBox::launchAsynchronously(
        std::move(colourSelector),
        getTopLevelComponent()->getLocalArea(this, getLocalBounds()),
        getTopLevelComponent()
    );
    
    callOutBox.setLookAndFeel(&getLookAndFeel());
}

void Patcher::parentHierarchyChanged() {
    if (auto* mainComponent = findParentComponentOfClass<MainComponent>())
        mainComponent->addMouseListener(this, true);
}
