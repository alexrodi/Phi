/*
  ==============================================================================

    MainPatcher.cpp
    Created: 29 Mar 2020 12:51:46am
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#include <JuceHeader.h>
#include "MainPatcher.h"

//==============================================================================
MainPatcher::MainPatcher() :
mainProcessor{std::make_unique<AudioProcessorGraph>()}
{
    // Menu and submenus content
    // Submenus must be filled before the main
    modulesSubMenu.addItem (1, "Impulse");
    modulesSubMenu.addItem (2, "Gain");
    modulesSubMenu.addItem (3, "Output");
    rightClickMenu.addSubMenu ("Add Module...", modulesSubMenu);
    
    addAndMakeVisible(connections);
    
    setSize(1000, 1000);
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
        
        // Eventually this should become a switch
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

void MainPatcher::togglePatchCordType()
{
    connections.togglePatchCordType();
}

// Registers all inlets and outlets with the connections component
void MainPatcher::registerInletsAndOutlets(Module* module, uint32 moduleId) {
    
    OwnedArray<phi_Inlet>& inlets = module->inlets;
    for (phi_Inlet* inlet : inlets)
    {
        inlet->setId(connections.registerInlet(moduleId, inlet));
        inlet->addActionListener(&connections);
    }
    
    OwnedArray<phi_Outlet>& outlets = module->outlets;
    for (phi_Outlet* outlet : outlets)
    {
        outlet->setId(connections.registerOutlet(moduleId, outlet));
        outlet->addActionListener(&connections);
    }
    
}

template <class moduleClass>
void MainPatcher::createModule(Point<float> position)
{
    ModuleBox* moduleBox;
    
    std::unique_ptr<Module> newModule = std::make_unique<moduleClass>();
    
    Module* modulePtr = newModule.get();
        
    moduleBox = new ModuleBox(modulePtr, selectedModules);
    
    modules.add(moduleBox);
    
    // Display and set its position
    addAndMakeVisible(moduleBox);
    moduleBox->setTopLeftPosition(position.toInt());
    moduleBox->addActionListener(&connections);
    
    AudioProcessorGraph::Node::Ptr newNode = mainProcessor->addNode(std::move(newModule));

    if (typeid(moduleClass) == typeid(module_Output))
    {
        // When we detect an output module,
        // we immediatelly hook it up to an AudioGraphIOProcessor in audioOutputNode mode
    
        AudioProcessorGraph::Node::Ptr outputNode = mainProcessor->addNode(std::make_unique<AudioProcessorGraph::AudioGraphIOProcessor>(AudioProcessorGraph::AudioGraphIOProcessor::audioOutputNode));
        
        for (int i = 0; i < modulePtr->props.inletNumber; i++)
        {
            mainProcessor->addConnection ({ { newNode->nodeID, i }, { outputNode->nodeID, i } });
        }
    }
    
    registerInletsAndOutlets(modulePtr, newNode.get()->nodeID.uid);
}

void MainPatcher::initialiseGraph()
{
    /** TODO - Find a proper place where to clear the graph (which isn't needed for now because there is no deletion of modules or connections) */
    // mainProcessor->clear();
    
    
    audioInputNode = mainProcessor->addNode (std::make_unique<AudioProcessorGraph::AudioGraphIOProcessor>(AudioProcessorGraph::AudioGraphIOProcessor::audioInputNode));
 
    audioOutputNode = mainProcessor->addNode (std::make_unique<AudioProcessorGraph::AudioGraphIOProcessor>(AudioProcessorGraph::AudioGraphIOProcessor::audioOutputNode));
    
    using Connection = std::pair<Connections::IOid, Connections::IOid>;
    
    Array<Connection> allConnections = connections.getAllConnectionIdPairs();
    
    if (! allConnections.isEmpty())
    {
        for (Connection connection : allConnections)
        {
            AudioProcessorGraph::NodeAndChannel source { {}, connection.first.second };
            AudioProcessorGraph::NodeAndChannel destination { {}, connection.second.second };
            
            source.nodeID.uid = connection.first.first;
            destination.nodeID.uid = connection.second.first;
            
            mainProcessor->addConnection ({ source, destination });
        }
    }
}

void MainPatcher::changeListenerCallback (ChangeBroadcaster* source)
{
    initialiseGraph();
}

