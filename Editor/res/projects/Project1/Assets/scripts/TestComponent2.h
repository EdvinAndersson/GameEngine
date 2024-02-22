#pragma once

#include "Core/src/ecs/GameObject.h"
#include "Core/src/ecs/Components.h"

struct TestComponent2 {
};

//void TestComponent_OnAwake(CW::GameObject game_object, TestComponent comp);
//void TestComponent_OnStart(CW::GameObject game_object, TestComponent comp);
//void TestComponent_OnUpdate(CW::GameObject game_object, TestComponent comp);
//void TestComponent_OnDestroy(CW::GameObject game_object, TestComponent comp);
extern "C" {
    #ifdef EXPORTING_DLL
        extern __declspec(dllexport) void TestComponent2_OnUpdate(CW::GameObject game_object, TestComponent2& comp);
    #else
        extern __declspec(dllimport) void TestComponent2_OnUpdate(CW::GameObject game_object, TestComponent2& comp);
    #endif
}