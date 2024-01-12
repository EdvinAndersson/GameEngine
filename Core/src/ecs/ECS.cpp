#include "ECS.h"

#include "EntityManager.h"
#include "ComponentManager.h"
#include "Components.h"

namespace CW {

    EntityManager *entityManager = new EntityManager();
    ComponentManager *componentManager = new ComponentManager();

    ECS::ECS() {
        componentManager->RegisterComponent<Transform>();
    }
}