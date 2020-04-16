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
MainPatcher::MainPatcher() :
tooltipWindow{this}
{
    setWantsKeyboardFocus(true);
    
    // Menu and submenus content
    // Submenus must be filled before the main
    modulesSubMenu.addItem (1, "Impulse");
    modulesSubMenu.addItem (2, "Gain");
    modulesSubMenu.addItem (3, "Output");
    rightClickMenu.addSubMenu ("Add Module...", modulesSubMenu);
    
    LookAndFeel::getDefaultLookAndFeel().setColour(TooltipWindow::outlineColourId, Colours::transparentBlack);
    LookAndFeel::getDefaultLookAndFeel().setColour(TooltipWindow::backgroundColourId, Colours::grey);
    LookAndFeel::getDefaultLookAndFeel().setColour(TooltipWindow::textColourId, Colours::darkgrey.darker());
    sendLookAndFeelChange();
    
    addAndMakeVisible(connections);
    connections.addChangeListener(this);
    
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
        
        if (result==1)
        {
            createModule<module_Impulse>(e.position);
        }
        else if (result==2)
        {
            createModule<module_Gain>(e.position);
        }
        else if (result==3)
        {
            createModule<module_Output>(e.position);
        }
    }
}

void MainPatcher::deleteModule(ModuleBox* moduleBox)
{
    connections.removeModule(moduleBox->module->nodeID.uid);
    audioEngine.removeNode(moduleBox->module->nodeID);
    modules.removeObject(moduleBox);
}

void MainPatcher::deleteAllSelectedModules()
{
    for (int i=0; i < selectedModules.getNumSelected(); i++)
        deleteModule(selectedModules.getSelectedItem(i));
}

bool MainPatcher::keyPressed (const KeyPress& key)
{
    if (key == KeyPress::backspaceKey)
    {
        deleteAllSelectedModules();
    }
    return true;
}

void MainPatcher::togglePatchCordType(bool toggle)
{
    connections.togglePatchCordType(toggle);
}

void MainPatcher::toggleInoutType(bool toggle)
{
    InletOptions::drawName = !toggle;
    OutletOptions::drawName = !toggle;
    
    repaint();
}

void MainPatcher::registerInletsAndOutlets(Module* module, uint32 moduleId)
{
    
    OwnedArray<Inlet>& inlets = module->inlets;
    for (Inlet* inlet : inlets)
    {
        inlet->setId(connections.registerInlet(moduleId, inlet));
        inlet->addActionListener(&connections);
    }
    
    OwnedArray<Outlet>& outlets = module->outlets;
    for (Outlet* outlet : outlets)
    {
        outlet->setId(connections.registerOutlet(moduleId, outlet));
        outlet->addActionListener(&connections);
    }
    
}

template <class moduleClass>
void MainPatcher::createModule(Point<float> position)
{
    std::unique_ptr<Module> newModule = std::make_unique<moduleClass>();
    
    Module* modulePtr = newModule.get();
        
    ModuleBox* moduleBox = new ModuleBox(modulePtr, selectedModules);
    
    modules.add(moduleBox);
    
    // Display and set its position
    addAndMakeVisible(moduleBox);
    moduleBox->setTopLeftPosition(position.toInt());
    moduleBox->addActionListener(&connections);
    
    AudioProcessorGraph::Node::Ptr newNode = audioEngine.addNode(std::move(newModule));

    // When we detect an output module, we must hook it up to the output node
    if (typeid(moduleClass) == typeid(module_Output))
    {
        audioEngine.connectToOuput(newNode);
    }
    
    modulePtr->nodeID = newNode->nodeID;
    
    registerInletsAndOutlets(modulePtr, newNode.get()->nodeID.uid);
}

void MainPatcher::changeListenerCallback (ChangeBroadcaster* source)
{
    if (source == &connections)
    {
        audioEngine.applyAudioConnections(connections.getAllConnectionIdPairs());
    }
}

