#pragma once

#include "Core/src/ecs/GameObject.h"
#include "Core/src/ecs/Components.h"

struct TestComponent2 {
};

extern CW::ComponentManager *_component_manager;
extern CW::EntityManager *_entity_manager;

void TestComponent2_OnAwake(CW::GameObject game_object, TestComponent2& comp);
void TestComponent2_OnStart(CW::GameObject game_object, TestComponent2& comp);
void TestComponent2_OnUpdate(CW::GameObject game_object, TestComponent2& comp);
void TestComponent2_OnDestroy(CW::GameObject game_object, TestComponent2& comp);