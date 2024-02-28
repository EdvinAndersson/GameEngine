#pragma once

#include "Core/src/ecs/GameObject.h"
#include "Core/src/ecs/Components.h"
#include "Editor/src/Console.h"

struct TestComponent {
};

void TestComponent_OnAwake(CW::GameObject game_object, TestComponent& comp);
void TestComponent_OnStart(CW::GameObject game_object, TestComponent& comp);
void TestComponent_OnUpdate(CW::GameObject game_object, TestComponent& comp);
void TestComponent_OnDestroy(CW::GameObject game_object, TestComponent& comp);