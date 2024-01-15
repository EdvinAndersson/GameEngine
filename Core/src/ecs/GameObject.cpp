#include "GameObject.h"
#include "../event/EventManager.h"

namespace CW {
    GameObject GameObject::Instantiate() {
        return Instantiate(Vec3 { 0.0f, 0.0f, 0.0f });
    }
    GameObject GameObject::Instantiate(Vec3 position) {
        Entity entity = entity_manager->CreateEntity();
        GameObject gameObject {};
        gameObject.entity = entity;
        Transform& transform = gameObject.AddComponent<Transform>();
        transform.position = position;
        transform.scale = Vec3 { 1.0f, 1.0f, 1.0f };

        EventData_ECS_INSTANTIATE_GAMEOBJECT e;
        e.game_object = gameObject;
        EventManager::InvokeEvent(ECS_INSTANTIATE_GAMEOBJECT, &e);

        return gameObject;
    }
    void GameObject::Destory(GameObject &gameObject) {
        entity_manager->DestroyEntity(gameObject.GetEntity());
        
        EventData_ECS_DESTROY_GAMEOBJECT e;
        e.game_object = gameObject;
        EventManager::InvokeEvent(ECS_DESTROY_GAMEOBJECT, &e);
    }
}