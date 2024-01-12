#include "GameObject.h"
#include "../event/EventManager.h"

namespace CW {

    GameObject GameObject::Instantiate() {
        Entity entity = entityManager->CreateEntity();
        GameObject gameObject {};
        gameObject.entity = entity;
        Transform& transform = gameObject.AddComponent<Transform>();
        transform.scale = Vec3 { 1.0f, 1.0f, 1.0f };

        EventData_ECS_INSTANTIATE_GAMEOBJECT e;
        e.game_object = gameObject;
        EventManager::InvokeEvent(ECS_INSTANTIATE_GAMEOBJECT, &e);

        return gameObject;
    }
    void GameObject::Destory(GameObject &gameObject) {
        entityManager->DestroyEntity(gameObject.GetEntity());
        
        EventData_ECS_DESTROY_GAMEOBJECT e;
        e.game_object = gameObject;
        EventManager::InvokeEvent(ECS_DESTROY_GAMEOBJECT, &e);
    }
}