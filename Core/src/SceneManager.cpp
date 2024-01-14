#include "SceneManager.h"

namespace CW {

    SceneManager::SceneManager() 
        : scene_count(0), current_scene_id(0) {
        EventListen(EventType::ECS_INSTANTIATE_GAMEOBJECT);
        EventListen(EventType::ECS_DESTROY_GAMEOBJECT);
    }

    void SceneManager::CreateNewScene(char* name) {
        Assert(scene_count < MAX_SCENES);

        Scene scene;
        strcpy(scene.name, name);

        scenes[scene_count++] = scene;
    }

    void SceneManager::SetActiveScene(char* scene_name) {
        for (unsigned int i = 0; i < scene_count; i++) {
            if (strcmp(scenes[i].name, scene_name) == 0) {
                current_scene_id = i;
                printf("Changed scene to %s\n", scene_name);
                return;
            }
        }
        Assert(false);
    }

    void SceneManager::OnEvent(Event event) {
        switch (event.event_type)
        {
            case EventType::ECS_INSTANTIATE_GAMEOBJECT:
            {
                EventData_ECS_INSTANTIATE_GAMEOBJECT *e = (EventData_ECS_INSTANTIATE_GAMEOBJECT*) event.data;

                scenes[current_scene_id].game_objects.insert(e->game_object);
            } break;
            case EventType::ECS_DESTROY_GAMEOBJECT:
            {
                EventData_ECS_DESTROY_GAMEOBJECT *e = (EventData_ECS_DESTROY_GAMEOBJECT*) event.data;
                scenes[current_scene_id].game_objects.erase(e->game_object);
            } break;            
        }
    }
}