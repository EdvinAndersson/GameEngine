#include "SceneManager.h"
#include "CWAssert.h"

namespace CW {

    SceneManager::SceneManager() 
        : current_scene_id(0) {
        EventListen(EventType::ECS_INSTANTIATE_GAMEOBJECT);
        EventListen(EventType::ECS_DESTROY_GAMEOBJECT);
        EventListen(EventType::PROJECT_LOAD);
    }

    void SceneManager::CreateNewScene(char* name) {
        CW_ASSERT(current_project != 0, "");

        CreateNewScene(current_project, name);
    }
    void SceneManager::CreateNewScene(Project *project, char* name) {
        CW_ASSERT(project->scene_count < MAX_SCENES, "");

        Scene scene;
        strcpy(scene.name, name);

        project->scenes[project->scene_count++] = scene;
    }

    void SceneManager::SetActiveScene(char* scene_name) {
        CW_ASSERT(current_project != 0, "");

        for (unsigned int i = 0; i < current_project->scene_count; i++) {
            if (strcmp(current_project->scenes[i].name, scene_name) == 0) {
                current_scene_id = i;
                printf("Changed scene to %s\n", scene_name);
                return;
            }
        }
        CW_ASSERT(false, "");
    }

    void SceneManager::OnEvent(Event event) {
        switch (event.event_type)
        {
            case EventType::PROJECT_LOAD:
            {
                EventData_PROJECT_LOAD *e = (EventData_PROJECT_LOAD *) event.data;
                current_project = e->project;
            } break;
            case EventType::ECS_INSTANTIATE_GAMEOBJECT:
            {
                CW_ASSERT(current_project != 0, "");

                EventData_ECS_INSTANTIATE_GAMEOBJECT *e = (EventData_ECS_INSTANTIATE_GAMEOBJECT*) event.data;

                current_project->scenes[current_scene_id].game_objects.insert(e->game_object);
            } break;
            case EventType::ECS_DESTROY_GAMEOBJECT:
            {
                CW_ASSERT(current_project != 0, "");

                EventData_ECS_DESTROY_GAMEOBJECT *e = (EventData_ECS_DESTROY_GAMEOBJECT*) event.data;
                current_project->scenes[current_scene_id].game_objects.erase(e->game_object);
            } break;            
        }
    }
    Scene& SceneManager::GetActiveScene() {
        CW_ASSERT(current_project != 0, "");
        
        return current_project->scenes[current_scene_id]; 
    }
    Scene* SceneManager::GetScenes() { 
        CW_ASSERT(current_project != 0, "");

        return current_project->scenes; 
    }
    unsigned int SceneManager::GetSceneCount() { 
        CW_ASSERT(current_project != 0, "");

        return current_project->scene_count; 
    }
}