#pragma once

#include "CWAssert.h"
#include "Utility.h"
#include <Shlwapi.h>

#include "Core/src/ecs/GameObject.h"

namespace CW {
    typedef void(*PFNInitGeneretedComponentsUtility)(ComponentManager *, EntityManager *);
    typedef void(*PFNRegisterGeneratedComponents)();
    typedef void(*PFNAddGenereatedComponent)(size_t type, GameObject& obj);
    typedef bool(*PFNHasGenereatedComponent)(size_t type, GameObject& obj);

    void BuildDLL();
    void FreeDLL();
    void LoadDLLFunctions(char *path);

    void InitGeneretedComponentsUtility();
    void RegisterGeneratedComponents();
    void AddGenereatedComponent(size_t type, GameObject& obj);
    bool HasGenereatedComponent(size_t type, GameObject& obj);
}