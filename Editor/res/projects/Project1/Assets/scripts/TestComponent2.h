#include "Core/src/ecs/GameObject.h"
#include "Core/src/ecs/Components.h"

struct TestComponent2 {
};

//void TestComponent_OnAwake(CW::GameObject game_object, TestComponent comp);
//void TestComponent_OnStart(CW::GameObject game_object, TestComponent comp);
//void TestComponent_OnUpdate(CW::GameObject game_object, TestComponent comp);
//void TestComponent_OnDestroy(CW::GameObject game_object, TestComponent comp);

extern __declspec(dllexport) inline void TestComponent2_OnUpdate(CW::GameObject game_object, TestComponent2 comp);