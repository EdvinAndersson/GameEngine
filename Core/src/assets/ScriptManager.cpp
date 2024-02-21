#include "ScriptManager.h"

#include "Windows.h"
#include "stdio.h"

namespace CW {

    void BuildDLL() {
        STARTUPINFOA si = {};
        PROCESS_INFORMATION pi = {};
        bool success = CreateProcessA(NULL, "tools\\build_scripts_dll.bat", NULL, NULL, false, 0, NULL, NULL, &si, &pi);
        
        CW_ASSERT(success != 0, "Could not build the game!");
    }
}