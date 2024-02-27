#include "GameObject.h"
#include "../event/EventManager.h"

namespace CW {
    GameObject GameObject::Instantiate() {
        return Instantiate(vec3s { 0.0f, 0.0f, 0.0f });
    }
    GameObject GameObject::Instantiate(vec3s position) {
        Entity entity = entity_manager->CreateEntity();
        GameObject gameObject {};
        gameObject.entity = entity;
        Transform& transform = gameObject.AddComponent<Transform>();
        transform.position = position;
        transform.scale = vec3s { 1.0f, 1.0f, 1.0f };

        EventData_ECS_INSTANTIATE_GAMEOBJECT e;
        e.game_object = gameObject;
        EventManager::InvokeEvent(ECS_INSTANTIATE_GAMEOBJECT, &e);

        return gameObject;
    }
    void GameObject::Destory(GameObject &gameObject) {
        component_manager->EntityDestroyed(gameObject.entity);
        entity_manager->DestroyEntity(gameObject.entity);
        
        EventData_ECS_DESTROY_GAMEOBJECT e;
        e.game_object = gameObject;
        EventManager::InvokeEvent(ECS_DESTROY_GAMEOBJECT, &e);
    }
}