/*
  ==============================================================================

    Patcher.cpp
    Created: 29 Mar 2020 12:51:46am
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#include "Patcher.h"
#include "MainComponent.h"

//==============================================================================
Patcher::Patcher(State& state) :
state(state),
connections(state, *this),
mouseListener(this)
{
    state.newModuleUICreated = [&] (auto ui, auto moduleID) {
        auto [it, inserted] = modules.insert({ moduleID,
            std::make_unique<ModuleBox>(state, moduleID, std::move(ui))
        });
        
        if (!inserted) return;
        
        addAndMakeVisible(*it->second);
        it->second->setShowPortLabels(showPortLabels);
    };
    
    setWantsKeyboardFocus(true);
    setPaintingIsUnclipped(true);
    
    addAndMakeVisible(connections);
    addAndMakeVisible(lasso);
    addChildComponent(hoverPopup);
    
    selectedModuleIDs.addChangeListener(this);
    state.addListener(this);
}

Patcher::~Patcher()
{
    state.removeListener(this);
}

const PortUI* Patcher::getPortUI (ModulePortID portID, PortType type) const {
    if (modules.contains(portID.moduleID))
        return &modules.at(portID.moduleID)->getPort(type, portID.portID);
    
    return nullptr;
}

std::optional<ModuleID> Patcher::getModuleID(const ModuleBox& box) const {
    for (auto& [moduleID, item] : modules)
        if (item.get() == &box)
            return {moduleID};
    
    return {};
}

std::optional<ModulePortID> Patcher::getModulePortID(const PortUI& port) const {
    if (auto box = port.findParentComponentOfClass<ModuleBox>()) {
        if (auto moduleID = getModuleID(*box))
            return {{*moduleID, box->getPortID(port)}};
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

void Patcher::onMouseDown(const juce::MouseEvent& e)
{
    // ============ Modules =======================
    
    if (auto box = dynamic_cast<ModuleBox*>(e.eventComponent))
    {
        if (auto moduleID = getModuleID(*box)) {
            if (e.mods.isRightButtonDown()) {
                openColourSelector(
                    (*box).getBounds(),
                    (*box).findColour(PhiColourIds::Module::Highlight),
                    this,
                    this
                );
            }
            
            selectedModuleIDs.addToSelectionOnMouseDown(*moduleID, e.mods);
            
            moduleDragger.addOnMouseDown(getSelectedModules());
            
            return;
        }
    }
    
    // ============ Patcher =======================
    
    if (e.eventComponent == this) {
        if (e.mods.isRightButtonDown())
            openMenu(e);
        else
            lasso.beginLasso(e, this);
    }
    
    
    // TODO: this isn't a reliable way to know if selection should be maintained
    // Probably set a flag when the colour selector is opened and have it callback when it's deleted to reset it
    bool clickIsFromCalloutBox = dynamic_cast<juce::CallOutBox*>(e.eventComponent)
        || e.eventComponent->findParentComponentOfClass<juce::CallOutBox>();
    
    // Only selection options popups (CallOutBox) maintain the selection
    if (!clickIsFromCalloutBox)
        selectedModuleIDs.deselectAll();
}

void Patcher::onMouseUp(const juce::MouseEvent& e)
{
    lasso.endLasso();
}

void Patcher::onMouseDrag(const juce::MouseEvent& e)
{
    if (e.eventComponent == this) {
        lasso.dragLasso(e);
    } else if (auto box = dynamic_cast<ModuleBox*>(e.eventComponent)) {
        moduleDragger.moveOnMouseDrag(e);
    }
}

bool Patcher::keyPressed (const juce::KeyPress& key)
{
    if (key == juce::KeyPress::backspaceKey)
    {
        for (auto moduleID : selectedModuleIDs)
            state.deleteModule(moduleID);
        
        selectedModuleIDs.deselectAll();
        connections.deleteAllSelected();
        
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

void Patcher::moduleBoundsChanged(ModuleID moduleID, const juce::Rectangle<int>& moduleBounds) {
    if (!modules.contains(moduleID)) return;
    
    // Don't allow modules to have negative positions
    auto bounds = moduleBounds.withPosition(
        std::max(0, moduleBounds.getX()),
        std::max(0, moduleBounds.getY())
    );
    
    if (bounds.isEmpty())
        // Set position only
        modules[moduleID]->setTopLeftPosition(bounds.getTopLeft());
    else
        modules[moduleID]->setBounds(bounds);
    
    // Fit content (only if bigger than window)
    setBounds(getParentComponent()->getLocalArea(this, getLocalBounds().getUnion(getContentBounds())));
}

void Patcher::moduleDeleted(ModuleID moduleID)
{
    modules.erase(moduleID);
}

void Patcher::showPortLabelsChanged(ShowPortLabels show) {
    showPortLabels = show;
    
    for (auto& [moduleID, box] : modules)
        box->setShowPortLabels(showPortLabels);
}

template<class CallbackType>
void Patcher::forEachSelected(CallbackType callback)
{
    for (auto& moduleID : selectedModuleIDs) {
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
        forEachSelected( [&] (auto moduleID, auto& _) {
            state.setModuleColour(moduleID, colourSelector->getCurrentColour());
        });
    }
}

void Patcher::parentHierarchyChanged() {
    if (auto* mainComponent = findParentComponentOfClass<MainComponent>())
        mainComponent->addMouseListener(&mouseListener, true);
}

juce::Rectangle<int> Patcher::getContentBounds() {
    juce::Rectangle<int> bounds;

    for (int i = 0; i < getNumChildComponents(); ++i)
        bounds = bounds.getUnion(getChildComponent(i)->getBounds());

    return bounds;
}
