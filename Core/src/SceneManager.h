#pragma once

#include "ecs/GameObject.h"
#include "event/IEventListener.h"

#include <set>

namespace CW {

    #define MAX_SCENES 16

    struct Scene {
        char name[1024];
        std::set<GameObject> game_objects;
    };

    class SceneManager : IEventListener {
        public:
            SceneManager();
            void CreateNewScene(char* name);
            void SetActiveScene(char* name);

            inline Scene& GetActiveScene() { return scenes[current_scene_id]; }
            inline Scene* GetScenes() { return scenes; }
            inline unsigned int GetSceneCount() { return scene_count; }
        private:
            void OnEvent(Event event) override;

            Scene scenes[MAX_SCENES];
            unsigned int scene_count, current_scene_id;
    };
}