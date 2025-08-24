/*
  ==============================================================================

    Connections.cpp
    Created: 20 Feb 2020 8:18:35pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#include "Connections.h"
#include "Patcher.h"
#include "MainComponent.h"

//==============================================================================

Connections::Connections(State& state, const Patcher& patcher) :
state(state),
patcher(patcher),
mouseListener(this)
{
    addChildComponent(lasso);
    
    state.addListener(this);
    selectedConnections.addChangeListener(this);
    
    setAlwaysOnTop(true);
    setPaintingIsUnclipped(true);
}

Connections::~Connections()
{
    state.removeListener(this);
}

void Connections::paint (juce::Graphics& g)
{
    // Paint the temporary drag path if a connection is being made
    if (heldConnection)
    {
        g.setColour ( heldConnection->colour );
        g.strokePath ( heldConnection->path, patchCordStroke );
    }
    
    for (auto& [id, connection] : connections)
    {
        g.setColour (connection.colour);
        g.fillPath (connection.path);
        
        if (selectedConnections.isSelected(id)) {
            g.setColour (findColour(PhiColourIds::Connection::SelectedOutline));
            g.strokePath (connection.path, selectedStroke);
        }
    }
}

void Connections::resized()
{
}


template<class CallbackType>
void Connections::forEachSelected(CallbackType callback)
{
    for (auto& connectionID : selectedConnections) {
        if (connections.contains(connectionID))
            callback(connectionID, connections[connectionID]);
    }
}

void Connections::deleteAllSelected() {
    forEachSelected([&] (auto id, auto _) { 
        state.deleteConnection(id);
    });
    
    selectedConnections.deselectAll();
}

void Connections::updateHeldConnectionPath(const juce::MouseEvent& e) {
    if (heldConnection) {
        auto getPath = patchCordType == PatchCordType::S ? getSPatchCordPath : getArcPatchCordPath;
        heldConnection->path = getPath({heldConnection->anchor, e.getPosition().toFloat()});
        repaint();
    }
}

void Connections::tryCreateHeldConnection(const juce::MouseEvent& e) {
    auto parent = getParentComponent();
    auto* originPort = dynamic_cast<PortUI*>(e.eventComponent);
    auto* destinationPort = dynamic_cast<PortUI*>(parent->getComponentAt(e.getEventRelativeTo(parent).position));
    
    if (originPort && destinationPort &&
        originPort->getType() != destinationPort->getType()) // Must be opposing types
    {
        auto originPortID = patcher.getModulePortID(*originPort);
        auto destinationPortID = patcher.getModulePortID(*destinationPort);
        
        if (originPortID && destinationPortID &&
            originPortID->moduleID != destinationPortID->moduleID) // Must be different modules
        {
            bool originIsOutlet = originPort->getType() == PortType::Outlet;
            
            state.createConnection({
                originIsOutlet ? *originPortID : *destinationPortID,
                originIsOutlet ? *destinationPortID: *originPortID
            });
        }
    }
}

void Connections::onMouseDown(const juce::MouseEvent& e)
{
    if (e.eventComponent == this)
    {
        selectedConnections.addToSelectionOnMouseDown(hitConnectionID, e.mods);
        
        if (e.mods.isRightButtonDown()) {
            openColourSelector(
                {e.position.toInt(), {1, 1}},
                connections[hitConnectionID].colour,
                this,
                this
            );
        }
        
        return;
    }
    
    if (auto port = dynamic_cast<PortUI*>(e.eventComponent))
    {
        if (auto portID = patcher.getModulePortID(*port)) {
            heldConnection = std::make_unique<HeldConnection>( HeldConnection {
                {.colour = findColour(PhiColourIds::Connection::DefaultFill)},
                .anchor = getLocalPoint(port, port->getLocalBounds().toFloat().getCentre())
            });
            
            updateHeldConnectionPath(e.getEventRelativeTo(this));
        }
    }
    
    if (auto patcher = dynamic_cast<Patcher*>(e.eventComponent)) {
        if (!e.mods.isRightButtonDown())
            lasso.beginLasso(e, this);
    }
    
    selectedConnections.deselectAll();
}

void Connections::onMouseUp(const juce::MouseEvent& e) {
    if (heldConnection) {
        tryCreateHeldConnection(e);
        
        // TODO: implement permanently held connection when shift is pressed
        heldConnection.reset();
        
        repaint();
    }
    
    lasso.endLasso();
}

void Connections::onMouseDrag(const juce::MouseEvent& e)
{
    if (dynamic_cast<PortUI*>(e.eventComponent)) {
        updateHeldConnectionPath(e.getEventRelativeTo(this));
    }
    
    if (auto patcher = dynamic_cast<Patcher*>(e.eventComponent))
        lasso.dragLasso(e);
}

void Connections::connectionCreated(ConnectionID connectionID) {
    connections[connectionID] = {.colour = findColour(PhiColourIds::Connection::DefaultFill)};
    updateConnectionPath(connectionID);
    repaint();
}

void Connections::connectionDeleted(ConnectionID connectionID) {
    connections.erase(connectionID);
    repaint();
}

void Connections::moduleBoundsChanged(ModuleID moduleID, juce::Rectangle<int> bounds) {
    for (auto& [id, _] : connections) {
        if (id.source.moduleID == moduleID || id.destination.moduleID == moduleID)
            updateConnectionPath(id);
    }
    
    repaint();
}

void Connections::patchCordTypeChanged(PatchCordType type) {
    patchCordType = type;
    
    for (auto& [id, _] : connections)
        updateConnectionPath(id);
    
    repaint();
}

void Connections::changeListenerCallback (juce::ChangeBroadcaster* source)
{
    if (auto colourSelector = dynamic_cast<juce::ColourSelector*>(source))
    {
        forEachSelected([&] (auto id, auto connection) {
            connection.colour = colourSelector->getCurrentColour();
        });
    }
    
    repaint();
}

bool Connections::hitTest(int x, int y) {
    for (auto& [id, connection] : connections) {
        auto bounds = connection.path.getBounds();
        
        // avoid obscuring ports
        if (bounds.getWidth() > 30.0f) bounds.reduce(10.0f, 0.0f);
        
        if (bounds.contains(x, y) && connection.path.contains(x, y)) {
            hitConnectionID = id;
            return true;
        }
    }
    return false;
}

juce::Path Connections::getArcPatchCordPath (const juce::Line<float>& line)
{
    auto middlePoint = line.getPointAlongLineProportionally(0.5f);
    middlePoint.addXY(0.0f, line.getLength() * CORD_WEIGHT);
    
    juce::Path path;
    
    path.startNewSubPath (line.getStart());
    path.cubicTo (line.getStart(), middlePoint, line.getEnd());
    
    return path;
}

juce::Path Connections::getSPatchCordPath (const juce::Line<float>& line)
{
    auto middlePoint = line.getPointAlongLineProportionally(0.5f);
    float hDistance = (line.getStartX() - line.getEndX()) * 0.2f;

    juce::Path path{};
    
    path.startNewSubPath (line.getStart());
    path.cubicTo (line.getStart() , line.getStart().translated(-hDistance, 0) , middlePoint);
    path.cubicTo (middlePoint , line.getEnd().translated(hDistance, 0) , line.getEnd());
    
    return path;
}

void Connections::updateConnectionPath(ConnectionID connectionID) {
    auto getPath = patchCordType == PatchCordType::S ? getSPatchCordPath : getArcPatchCordPath;
    
    auto* outlet = patcher.getPortUI(connectionID.source, PortType::Outlet);
    auto* inlet = patcher.getPortUI(connectionID.destination, PortType::Inlet);
    
    if (outlet && inlet) {
        auto path = getPath({
            getLocalPoint(outlet, outlet->getLocalBounds().toFloat().getCentre()),
            getLocalPoint(inlet, inlet->getLocalBounds().toFloat().getCentre())
        });
        
        patchCordStroke.createStrokedPath(path, path);
        
        connections[connectionID].path = path;
    }
}

void Connections::findLassoItemsInArea (juce::Array<ConnectionID>& itemsFound, const juce::Rectangle<int>& area)
{
    for (auto& [id, connection] : connections) {
        float length = connection.path.getLength();
        float distance = 0.0f;
        while (distance < length) {
            if (area.contains(connection.path.getPointAlongPath(distance += 10.0f).toInt())) {
                itemsFound.add(id);
                break;
            }
        }
    }
}

void Connections::parentHierarchyChanged() {
    if (auto* mainComponent = findParentComponentOfClass<MainComponent>())
        mainComponent->addMouseListener(&mouseListener, true);
    
    // addGlobalMouseListener ???maybe???
}
 
// TODO: LASSO!

juce::SelectedItemSet<ConnectionID>& Connections::getLassoSelection() {return selectedConnections;}
