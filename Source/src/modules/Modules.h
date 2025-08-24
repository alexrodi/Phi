/*
  ==============================================================================

    Modules.h
    Created: 3 Apr 2020 4:57:53pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

#include "Gain/GainProcessor.h"
#include "Impulse/ImpulseProcessor.h"
#include "Output/OutputProcessor.h"
#include "String/StringProcessor.h"
#include "Grit/GritProcessor.h"
#include "Friction/FrictionProcessor.h"
#include "LFO/LFOProcessor.h"
#include "Filter/FilterProcessor.h"
// Add Module processor headers here

using ModuleTypeList = std::tuple<LFOProcessor,
                                 ImpulseProcessor,
                                 FrictionProcessor,
                                 GritProcessor,
                                 StringProcessor,
                                 FilterProcessor,
                                 GainProcessor,
                                 OutputProcessor>;

const std::vector<std::string> moduleNames = {
    "LFO",
    "Impulse",
    "Friction",
    "Grit",
    "String",
    "Filter",
    "Gain",
    "Output"
};

struct Modules {
    static juce::PopupMenu getMenu() {
        juce::PopupMenu modulesMenu;
        
        int index = 1;
        for (const auto& name : moduleNames)
            modulesMenu.addItem(index++, name);
        
        return modulesMenu;
    }

    static std::unique_ptr<ModuleInfo> getInfoFromMenuIndex(int index) {
        if (index >= 0 && index < moduleNames.size())
            return std::move(moduleList()[index]);
        
        // Module not found!
        jassertfalse;
        return {};
    }
    
private:
    template<class ProcessorType>
    struct ModuleEntry : public ModuleInfo {
        ModuleEntry(juce::String name) : ModuleInfo(name) {}
        std::unique_ptr<ModuleProcessor> create() override {
            return std::make_unique<ProcessorType>();
        }
        ~ModuleEntry() override = default;
    };

    static std::vector<std::unique_ptr<ModuleInfo>> moduleList() {
        std::vector<std::string> names (moduleNames.begin(), moduleNames.end());
        
        std::vector<std::unique_ptr<ModuleInfo>> list;
        list.reserve(std::tuple_size_v<ModuleTypeList>);

        [&]<size_t... I>(std::index_sequence<I...>) {
            (list.emplace_back(std::make_unique<ModuleEntry<std::tuple_element_t<I, ModuleTypeList>>>(names[I])), ...);
        }(std::make_index_sequence<std::tuple_size_v<ModuleTypeList>>{});

        return list;
    }
};
