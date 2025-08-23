/*
  ==============================================================================

    Connections.cpp
    Created: 20 Feb 2020 8:18:35pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#include "Connections.h"
#include "Patcher.h"

//==============================================================================

Connections::Connections(State& state, const Patcher& patcher) :
state(state),
patcher(patcher)
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
        g.setColour ( findColour(PhiColourIds::Connection::DefaultFill) );
        g.strokePath ( heldConnection->path, patchCordStroke );
    }
    
    for (auto& [id, connection] : connections)
    {
        g.setColour (connection.colour);
        g.fillPath (connection.path);
        
        if (selectedConnections.isSelected(id)) {
            g.setColour (findColour(PhiColourIds::Connection::SelectedStroke));
            g.strokePath (connection.path, selectedStroke);
        }
    }
}

void Connections::resized()
{
}

void Connections::portMouseDown(ModulePortID modulePortID, PortType type) {
    if (heldConnection)
    {
        completeHeldConnection(modulePortID, type);
    }
    else if (auto* portUI = patcher.getPortUI(modulePortID, type))
    {
        heldConnection = std::make_unique<HeldConnection>();
        heldConnection->originType = type;
        heldConnection->originID = modulePortID;
        heldConnection->anchor = getLocalPoint(portUI, portUI->getLocalBounds().getCentre().toFloat());
    }
        
    repaint();
}


void Connections::portMouseUp(ModulePortID modulePortID, PortType type) {
    if (heldConnection)
        completeHeldConnection(modulePortID, type);
}

void Connections::portMouseMove(ModulePortID modulePortID, PortType type, const juce::MouseEvent& e) {
    updateHeldConnectionPath(e);
}

void Connections::completeHeldConnection(ModulePortID modulePortID, PortType type) {
    if (heldConnection->originType != type) {
        bool originIsOutlet = heldConnection->originType == PortType::Outlet;
        
        state.createConnection({
            originIsOutlet ? heldConnection->originID : modulePortID,
            originIsOutlet ? modulePortID: heldConnection->originID
        });
    }
    
    heldConnection.reset();
}

void Connections::updateHeldConnectionPath(const juce::MouseEvent& e) {
    if (heldConnection) {
        auto getPath = patchCordType == PatchCordType::S ? getSPatchCordPath : getArcPatchCordPath;
        heldConnection->path = getPath({heldConnection->anchor, e.getPosition().toFloat()});
        repaint();
    }
}

void Connections::openColourSelector(juce::Point<int> point, juce::Colour initialColour)
{
    auto colourSelector = std::make_unique<juce::ColourSelector>(juce::ColourSelector::showColourspace);
    colourSelector->setSize (150, 130);
    colourSelector->addChangeListener(this);
    colourSelector->setCurrentColour(initialColour);

    auto& callOutBox = juce::CallOutBox::launchAsynchronously(
        std::move(colourSelector),
        getTopLevelComponent()->getLocalArea(this, juce::Rectangle<int>(point.x, point.y, 1, 1)),
        getTopLevelComponent()
    );
    
    callOutBox.setLookAndFeel(&getLookAndFeel());
}

template<class CallbackType>
void Connections::forEachSelected(CallbackType callback)
{
    for (auto& connectionID : selectedConnections.getItemArray()) {
        if (connections.contains(connectionID))
            callback(connectionID, connections[connectionID]);
    }
}

void Connections::mouseDown(const juce::MouseEvent& e)
{
    if (!e.mods.isShiftDown())
        selectedConnections.deselectAll();
        
    selectedConnections.addToSelectionOnMouseDown(hitConnectionID, e.mods);
    
    if (e.mods.isRightButtonDown())
        openColourSelector(e.position.toInt(), connections[hitConnectionID].colour);
    
    lasso.endLasso();
}

void Connections::mouseMove(const juce::MouseEvent& e)
{
    updateHeldConnectionPath(e);
}

void Connections::connectionCreated(ConnectionID connectionID) {
    connections[connectionID] = {.colour = findColour(PhiColourIds::Connection::DefaultFill)};
    updateConnectionPath(connectionID);
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
        if (connection.path.contains(x, y)) {
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
        connections[connectionID].path = getPath({
            getLocalPoint(outlet, outlet->getLocalBounds().getCentre().toFloat()),
            getLocalPoint(inlet, inlet->getLocalBounds().getCentre().toFloat())
        });
    }
}

void Connections::findLassoItemsInArea (juce::Array<ConnectionID>& itemsFound, const juce::Rectangle<int>& area)
{
    for (auto& [id, connection] : connections) {
        float length = connection.path.getLength();
        float distance = 0.0f;
        while (distance < length){
            if (area.contains(connection.path.getPointAlongPath(distance += 10.0f).toInt()))
            {
                itemsFound.add(id);
                break;
            }
        }
    }
}

juce::SelectedItemSet<ConnectionID>& Connections::getLassoSelection() {return selectedConnections;}
