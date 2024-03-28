#pragma once

#include "Core/src/ecs/GameObject.h"
#include "Core/src/ecs/Components.h"

struct PlayerMovement {
};

extern CW::ComponentManager *_component_manager;
extern CW::EntityManager *_entity_manager;
void PlayerMovement_OnAwake(CW::GameObject game_object, PlayerMovement& comp);
void PlayerMovement_OnStart(CW::GameObject game_object, PlayerMovement& comp);
void PlayerMovement_OnUpdate(CW::GameObject game_object, PlayerMovement& comp);
void PlayerMovement_OnDestroy(CW::GameObject game_object, PlayerMovement& comp);
