#pragma once

#include "CWAssert.h"
#include "Utility.h"
#include <Shlwapi.h>

#include "Core/src/ecs/GameObject.h"

#include "Editor/src/vendor/framed/framed.h"

namespace CW {
    typedef void(*PFNInitGeneretedComponentsUtility)(ComponentManager *, EntityManager *);
    typedef void(*PFNRegisterGeneratedComponents)();
    typedef void(*PFNUpdateGeneratedComponents)();
    typedef void(*PFNAddGenereatedComponent)(size_t type, GameObject& obj);
    typedef bool(*PFNHasGenereatedComponent)(size_t type, GameObject& obj);


    void BuildDLL();
    void FreeDLL();
    void LoadDLLFunctions();

    void InitGeneretedComponentsUtility();
    void RegisterGeneratedComponents();
    void UpdateGeneratedComponents();
    void AddGenereatedComponent(size_t type, GameObject& obj);
    bool HasGenereatedComponent(size_t type, GameObject& obj);
}