/*
  ==============================================================================

    Module.h
    Created: 29 Mar 2020 11:38:39am
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "UIobjects/phi_Inlet.h"
#include "UIobjects/phi_Outlet.h"

//==============================================================================
/*
 Module - The base class for all modules
*/
class Module    : public Component
{
public:
    
    struct Arguments
    {
        String name;
        int inletNumber;
        int outletNumber;
        int width;
        int height;
    };
    
    Module(Arguments);
    
    const String name;
    
    const int width;
    const int height;
    
    /// This is how we keep our inlets & outlets,
    /// they belong to the module itself (not the box) and they should be public
    OwnedArray<phi_Inlet> inlets;
    OwnedArray<phi_Outlet> outlets;
    
    const Rectangle<int> placeInletsOutlets (Rectangle<int>);

private:
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Module)
};
