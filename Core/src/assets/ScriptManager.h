#pragma once

#include "CWAssert.h"
#include "Utility.h"
#include <Shlwapi.h>

#include "Core/src/ecs/GameObject.h"

namespace CW {
    typedef void(*PFNInitGeneratedComponentsUtility)(ComponentManager *, EntityManager *);
    typedef void(*PFNRegisterGeneratedComponents)();
    typedef void(*PFNUpdateGeneratedComponents)();
    typedef void(*PFNAddGeneratedComponent)(size_t type, GameObject& obj);
    typedef bool(*PFNHasGeneratedComponent)(size_t type, GameObject& obj);
    typedef void(*PFNRemoveGeneratedComponent)(size_t type, GameObject& obj);


    void BuildDLL();
    void FreeDLL();
    void LoadDLLFunctions();

    void InitGeneratedComponentsUtility();
    void RegisterGeneratedComponents();
    void UpdateGeneratedComponents();
    void AddGeneratedComponent(size_t type, GameObject& obj);
    bool HasGeneratedComponent(size_t type, GameObject& obj);
    void RemoveGeneratedComponent(size_t type, GameObject& obj);
}