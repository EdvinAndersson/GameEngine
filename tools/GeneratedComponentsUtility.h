#pragma once

#include "Core/src/Utility.h"
#include "Core/src/ecs/GameObject.h"

extern "C" {
    #ifdef EXPORTING_DLL
        extern __declspec(dllexport) void InitGeneretedComponentsUtility(CW::ComponentManager *component_manager, CW::EntityManager *entity_manager);
        extern __declspec(dllexport) void RegisterGeneratedComponents();
        extern __declspec(dllexport) void AddGenereatedComponent(size_t type, CW::GameObject& obj);
        extern __declspec(dllexport) bool HasGenereatedComponent(size_t type, CW::GameObject& obj);
    #else
        extern __declspec(dllimport) void InitGeneretedComponentsUtility(CW::ComponentManager *component_manager, CW::EntityManager *entity_manager);
        extern __declspec(dllimport) void RegisterGeneratedComponents();
        extern __declspec(dllimport) void AddGenereatedComponent(size_t type, CW::GameObject& obj);
        extern __declspec(dllimport) bool HasGenereatedComponent(size_t type, CW::GameObject& obj);
    #endif
};