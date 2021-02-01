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
    
    // Menu and submenus content
    // Submenus must be filled before the main
    // #TODO Obviously there will be a better way to do this for more modules
    modulesSubMenu.addItem (1, "Impulse");
    modulesSubMenu.addItem (2, "String");
    modulesSubMenu.addItem (3, "Gain");
    modulesSubMenu.addItem (4, "Output");
    rightClickMenu.addSubMenu ("Add Module...", modulesSubMenu);
    
    addAndMakeVisible(connections);
    connections.addChangeListener(this);
    selectedModules.addChangeListener(this);
    
    setSize(1000, 1000);
    setPaintingIsUnclipped(true);
}

MainPatcher::~MainPatcher()
{
    rightClickMenu.dismissAllActiveMenus();
    modulesSubMenu.dismissAllActiveMenus();
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
    // Deselect all modules when clicking the window
    selectedModules.deselectAll();
    
    // Right Click
    if (e.mods.isRightButtonDown()){
        
        // Displays the menu and returns the ID of the selected item (0 if clicked outside)
        const int result = rightClickMenu.showMenu(PopupMenu::Options().withParentComponent(getParentComponent()));
        
        // #TODO Obviously there will be a better way to do this for more modules
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

    } else {
        // Hand over event to connections
        connections.onMouseDown(e);
    }
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
        return true;
    } else {
        connections.keyPressed(key);
    }
    return false;
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
    
    auto moduleBox = new ModuleBox(std::move(moduleUI), selectedModules);
    modules.add(moduleBox);
    
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
    else if (source == &selectedModules && selectedModules.getNumSelected() > 0)
    {
        connections.deselectAll();
    }
    else if (modules.contains(dynamic_cast<ModuleBox*>(source)))
    {
        connections.refresh();
    }
}
