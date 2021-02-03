/*
  ==============================================================================

    MainPatcher.cpp
    Created: 29 Mar 2020 12:51:46am
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

///@cond
#include <JuceHeader.h>
///@endcond
#include "MainPatcher.h"

//==============================================================================
MainPatcher::MainPatcher(std::unique_ptr<AudioEngine> audioEngineToUse) :
audioEngine(std::move(audioEngineToUse)),
tooltipWindow{this}
{
    setWantsKeyboardFocus(true);
    
    addAndMakeVisible(connections);
    addAndMakeVisible(lasso);
    
    addMouseListener(&connections, true);
    
    connections.addChangeListener(this);
    selectedModules.addChangeListener(this);
    
    setSize(1000, 1000);
    setPaintingIsUnclipped(true);
}

MainPatcher::~MainPatcher()
{
}


//==============================================================================
void MainPatcher::paint (Graphics& g)
{
}

void MainPatcher::resized()
{
    connections.setBounds(getLocalBounds());
}

void MainPatcher::mouseDown(const MouseEvent& e)
{
    if (!e.mods.isShiftDown())
        selectedModules.deselectAll();
    
    // Right Click
    if (e.mods.isRightButtonDown() && !connections.onMouseRightButton(e)){
        openMenu(e);
    } else {
        lasso.beginLasso(e, this);
        connections.lasso.beginLasso(e, &connections);
    }
}

void MainPatcher::mouseUp(const MouseEvent& e)
{
    lasso.endLasso();
    connections.lasso.endLasso();
}

void MainPatcher::mouseDrag(const MouseEvent& e)
{
    lasso.dragLasso(e);
    connections.lasso.dragLasso(e);
}

void MainPatcher::openMenu(const MouseEvent& e)
{
    // #TODO Obviously there will be a better way to do this for more modules
    
    PopupMenu rightClickMenu;
    PopupMenu modulesSubMenu;
    
    modulesSubMenu.addItem (1, "Impulse");
    modulesSubMenu.addItem (2, "String");
    modulesSubMenu.addItem (3, "Gain");
    modulesSubMenu.addItem (4, "Output");
    rightClickMenu.addSubMenu ("Add Module...", modulesSubMenu);
    
    // Returns the ID of the selected item (0 if clicked outside)
    rightClickMenu.showMenuAsync(PopupMenu::Options().withParentComponent(this), [this, e] (int result) {
        if (result==1)
        {
            createModule<ImpulseProcessor>(e.position);
        }
        else if (result==2)
        {
            createModule<StringProcessor>(e.position);
        }
        else if (result==3)
        {
            createModule<GainProcessor>(e.position);
        }
        else if (result==4)
        {
            createModule<OutputProcessor>(e.position);
        }
    });
}

void MainPatcher::deleteModule(ModuleBox* moduleBox)
{
    connections.removeModule(moduleBox->moduleUI->nodeID.uid);
    audioEngine->removeNode(moduleBox->moduleUI->nodeID);
    modules.removeObject(moduleBox);
}

void MainPatcher::deleteAllSelectedModules()
{
    for (int i=0; i < selectedModules.getNumSelected(); i++)
        deleteModule(selectedModules.getSelectedItem(i));
    
    selectedModules.deselectAll();
}

bool MainPatcher::keyPressed (const KeyPress& key)
{
    if (selectedModules.getNumSelected() > 0 && key == KeyPress::backspaceKey){
        deleteAllSelectedModules();
    }
    return connections.keyPressed(key);
}

void MainPatcher::togglePatchCordType(bool toggle)
{
    connections.togglePatchCordType(toggle);
}

void MainPatcher::toggleInoutType(bool toggle)
{
    PlugOptions::drawName = toggle;
    sendLookAndFeelChange();
}

void MainPatcher::registerPlugs(OwnedArray<Plug>& plugArray, uint32 moduleID)
{
    for (auto& plug : plugArray)
    {
        plug->setId(connections.registerPlug(moduleID, plug));
        plug->addListener(&connections);
    }
}

void MainPatcher::registerInletsAndOutlets(ModuleUI& module)
{
    registerPlugs(module.inlets, module.nodeID.uid);
    registerPlugs(module.outlets, module.nodeID.uid);
}

template <class moduleClass>
void MainPatcher::createModule(Point<float> position)
{
    auto moduleProcessor = std::make_unique<moduleClass>();
    
    auto moduleUI = moduleProcessor->createUI();
    
    auto newNode = audioEngine->addNode(std::move(moduleProcessor));
    
    // Set is_output flag for output modules
    newNode->properties.set("is_output", typeid(moduleClass) == typeid(OutputProcessor));
    
    moduleUI->nodeID = newNode->nodeID;
    
    registerInletsAndOutlets(*moduleUI);
    
    auto moduleBox = modules.add(
        std::make_unique<ModuleBox>(std::move(moduleUI), selectedModules)
    );
    
    // Display and set its position
    addAndMakeVisible(moduleBox);
    moduleBox->setTopLeftPosition(position.toInt());
    moduleBox->addChangeListener(this);
}

void MainPatcher::changeListenerCallback (ChangeBroadcaster* source)
{
    if (source == &connections)
    {
        audioEngine->applyAudioConnections(connections.getConnections());
    }
    else if (source == &selectedModules && selectedModules.getNumSelected() > 0 && !isMouseOverOrDragging())
    {
        connections.deselectAll();
    }
    else if (modules.contains(dynamic_cast<ModuleBox*>(source)))
    {
        connections.refresh();
    }
}

void MainPatcher::findLassoItemsInArea (Array<ModuleBox*>& itemsFound, const Rectangle<int>& area)
{
    for (auto& module : modules)
        if (module->getBounds().intersects(area))
            itemsFound.add(module);
}

SelectedItemSet<ModuleBox*>& MainPatcher::getLassoSelection() {return selectedModules;}
