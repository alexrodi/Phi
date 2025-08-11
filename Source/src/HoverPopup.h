
#pragma once

///@cond
#include <JuceHeader.h>
///@endcond

class HoverPopupClient {
public:
    explicit HoverPopupClient(juce::Component* parentComponent) : parent(parentComponent), listener(this)
    {
        parent->addMouseListener(&listener, true);
    }
    
    virtual ~HoverPopupClient () {
        parent->removeMouseListener(&listener);
    };
    
    bool wantsToShow() { return shouldBeShowing; }
    
    void setHoverPopupEnabled (bool shouldBeEnabled) {enabled = shouldBeEnabled;}
    
    /// Override this to return the local center position to show the pop-up at.
    /// Position can be negative in relation to this component as it will presumably be drawn in a parent.
    virtual juce::Point<float> hoverPopupPosition() = 0;
    
    virtual juce::String getPopupText() = 0;
    
    /// Assign a custom position for the pop-up for specific instances' behaviour
    /// The one argument are the local bounds of the component
    std::function<juce::Point<float>(const juce::Rectangle<int>&)> customHoverPopupPosition;
    
private:
    juce::Component* parent;
    
    struct Listener : juce::MouseListener {
        explicit Listener(HoverPopupClient* owner) : owner(owner) {}
        
        void mouseEnter (const juce::MouseEvent&) override { owner->show(true); }
        void mouseMove (const juce::MouseEvent&) override { owner->show(true); }
        void mouseDrag (const juce::MouseEvent&) override { owner->show(true); }
        void mouseExit (const juce::MouseEvent&) override { owner->show(false); }
    
    private:
        HoverPopupClient* owner;
    } listener;
    
    bool shouldBeShowing = false;
    bool enabled = true;
    
    juce::Point<float> getHoverPopupPosition(const juce::Rectangle<int>& bounds) {
        if (customHoverPopupPosition) return customHoverPopupPosition(bounds);
        else return hoverPopupPosition();
    }
    
    void show (bool shouldShow) {
        shouldBeShowing = shouldShow ? enabled : false;
    }
    
    friend class HoverPopupWindow;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HoverPopupClient)
};

class HoverPopupWindow : public juce::Component,
                         public juce::Timer
{
public:
    HoverPopupWindow()
    {
        setPaintingIsUnclipped(true);
        setInterceptsMouseClicks(false, false);
        startTimer(50);
    }
    
    void paint(juce::Graphics& g) override
    {
        g.setColour(backgroundColour);
        if (cornerRadius > 0.0f) g.fillRoundedRectangle(getLocalBounds().toFloat(), cornerRadius);
        else g.fillRect(getLocalBounds());
        
        tl.draw(g, getLocalBounds().toFloat());
    }
    
    void timerCallback() override
    {
        bool isShowing = traverseAndShowPopup(getParentComponent());
        if ( !isShowing ) setVisible(false);
    }

    bool traverseAndShowPopup(juce::Component* parent)
    {
        // HoverPopupWindow must be added as a child of a component!
        jassert(parent != nullptr);
        
        for (auto child : parent->getChildren())
        {
            // Bail if we find another window in a sub-tree of children (allows for nested HoverPopupWindows)
            if (child != this && dynamic_cast<HoverPopupWindow*>(child)) return false;
            
            if (auto client = dynamic_cast<HoverPopupClient*>(child))
            {
                if (client->wantsToShow())
                {
                    if (isVisible()) {
                        showPopup(*client, child->getName());
                        return true;
                    } else {
                        stopTimer();
                        callAfterDelay(popupDelayMs, [this] (){
                            setVisible(true);
                            timerCallback();
                            startTimer(50);
                        });
                    }
                    return false;
                }
            }
            else if (child->getChildren().size() > 0)
            {
                if (traverseAndShowPopup(child)) return true;
            }
        }
        return false;
    }
    
    void setShowComponentNames( bool shouldShowComponentNames )
    {
        showComponentName = shouldShowComponentNames;
    }
    
    void setCornerRadius (float newCornerRadius) {
        cornerRadius = newCornerRadius;
    }
    
private:
    int popupDelayMs = 500;
    float cornerRadius = 3.0f;
    bool showComponentName = false;
    
    void showPopup(HoverPopupClient& client, const juce::String& componentName)
    {
        if (auto clientComponent = dynamic_cast<Component*>(&client))
        {
            backgroundColour = findColour(TooltipWindow::backgroundColourId);
            
            auto text = (showComponentName ? componentName + ": " : "") + client.getPopupText();
            
            juce::AttributedString s;
            s.setJustification (juce::Justification::centred);
            s.append (text, juce::Font{FontOptions{14.0f}}, findColour(TooltipWindow::textColourId));

            tl.createLayoutWithBalancedLineLengths (s, s.getText().length() * 13);
        
            auto position = getParentComponent()->getLocalPoint(clientComponent, client.getHoverPopupPosition(clientComponent->getLocalBounds()));
            int w = tl.getWidth() + 8;
            int h = tl.getHeight() + 6;
            
            setSize(w, h);
            setCentrePosition(position.x, position.y);
            toFront(false);
            
            repaint();
        }
    }
    
    juce::TextLayout tl;
    juce::Colour backgroundColour;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HoverPopupWindow);
};

