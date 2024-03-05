#pragma once

#include "Core/src/Utility.h"
#include "Core/src/ecs/GameObject.h"

extern "C" {
    #ifdef EXPORTING_DLL
        extern __declspec(dllexport) void InitGeneratedComponentsUtility(CW::ComponentManager *component_manager, CW::EntityManager *entity_manager);
        extern __declspec(dllexport) void RegisterGeneratedComponents();
        extern __declspec(dllexport) void UpdateGeneratedComponents();
        extern __declspec(dllexport) void AddGeneratedComponent(size_t type, CW::GameObject& obj);
        extern __declspec(dllexport) bool HasGeneratedComponent(size_t type, CW::GameObject& obj);
        extern __declspec(dllexport) void RemoveGeneratedComponent(size_t type, CW::GameObject& obj);
    #else
        extern __declspec(dllimport) void InitGeneratedComponentsUtility(CW::ComponentManager *component_manager, CW::EntityManager *entity_manager);
        extern __declspec(dllimport) void RegisterGeneratedComponents();
        extern __declspec(dllimport) void UpdateGeneratedComponents();
        extern __declspec(dllimport) void AddGeneratedComponent(size_t type, CW::GameObject& obj);
        extern __declspec(dllimport) bool HasGeneratedComponent(size_t type, CW::GameObject& obj);
        extern __declspec(dllimport) void RemoveGeneratedComponent(size_t type, CW::GameObject& obj);
    #endif
};