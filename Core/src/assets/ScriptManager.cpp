#include "ScriptManager.h"

#include "Windows.h"
#include "stdio.h"
#include <string>

namespace CW {

    PFNInitGeneretedComponentsUtility _InitGeneretedComponentsUtility;
    PFNRegisterGeneratedComponents _RegisterGeneratedComponents;
    PFNAddGenereatedComponent _AddGenereatedComponent;
    PFNHasGenereatedComponent _HasGenereatedComponent;
    PFNUpdateGeneratedComponents _UpdateGeneratedComponents;

    HINSTANCE hDll;

    void PrintErrorMessage(DWORD errorMessageID) {
        LPSTR messageBuffer = nullptr;
        if(errorMessageID != 0) {
            size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                    NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);
            std::string message(messageBuffer, size);
            printf("[WIN ERROR]: %s\n", message.c_str());
        }
    }

    void BuildDLL() {
        STARTUPINFOA si = {};
        PROCESS_INFORMATION pi = {};
        bool success = CreateProcessA(NULL, "tools\\build_scripts_dll.bat", NULL, NULL, false, 0, NULL, NULL, &si, &pi);
        CW_ASSERT(success != 0, "Could not build the game!");
        
        DeleteFileA("tools/build_scripts/ScriptsDLL.lib");
        DeleteFileA("tools/build_scripts/ScriptsDLL.dll");

        DWORD errorMessageID = 1;
        while(errorMessageID != 0) {
            success = CopyFileA("tools/build_scripts/ScriptsDLL.dll", "Editor/build/ScriptsDLL.dll", FALSE);
            errorMessageID = GetLastError();
            //PrintErrorMessage(errorMessageID);
            Sleep(0);
        }
        errorMessageID = 1;
        while(errorMessageID != 0) {
            success = CopyFileA("tools/build_scripts/ScriptsDLL.lib", "Editor/build/ScriptsDLL.lib", FALSE);
            errorMessageID = GetLastError();
            //PrintErrorMessage(errorMessageID);
            Sleep(0);
        }
    }

    void FreeDLL() {
        if (hDll == 0) return;

        FreeLibrary(hDll);
    }

    void LoadDLLFunctions() {
        hDll = LoadLibraryA("ScriptsDLL.dll");
        CW_ASSERT(hDll != 0, "Could not link Scripts DLL");

        _InitGeneretedComponentsUtility = (PFNInitGeneretedComponentsUtility) GetProcAddress(hDll, "InitGeneretedComponentsUtility");
        CW_ASSERT(_InitGeneretedComponentsUtility, "Unable to load function pointer!");

        _RegisterGeneratedComponents = (PFNRegisterGeneratedComponents) GetProcAddress(hDll, "RegisterGeneratedComponents");
        CW_ASSERT(_RegisterGeneratedComponents, "Unable to load function pointer!");

        _UpdateGeneratedComponents = (PFNUpdateGeneratedComponents) GetProcAddress(hDll, "UpdateGeneratedComponents");
        CW_ASSERT(_UpdateGeneratedComponents, "Unable to load function pointer!");

        _AddGenereatedComponent = (PFNAddGenereatedComponent) GetProcAddress(hDll, "AddGenereatedComponent");
        CW_ASSERT(_AddGenereatedComponent, "Unable to load function pointer!");

        _HasGenereatedComponent = (PFNHasGenereatedComponent) GetProcAddress(hDll, "HasGenereatedComponent");
        CW_ASSERT(_HasGenereatedComponent, "Unable to load function pointer!");
    }

    void InitGeneretedComponentsUtility() { _InitGeneretedComponentsUtility(component_manager, entity_manager); }
    void RegisterGeneratedComponents() { _RegisterGeneratedComponents(); }
    void UpdateGeneratedComponents() { _UpdateGeneratedComponents(); }
    void AddGenereatedComponent(size_t type, GameObject& obj) { _AddGenereatedComponent(type, obj); }
    bool HasGenereatedComponent(size_t type, GameObject& obj) { return _HasGenereatedComponent(type, obj); }
}