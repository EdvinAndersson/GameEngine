#pragma once

#include "../ecs/GameObject.h"
#include "../event/IEventListener.h"
#include "../project/Project.h"
#include "Scene.h"

#include <set>

namespace CW {

    class SceneManager : IEventListener {
        public:
            SceneManager();
            void CreateNewScene(char* name);
            void CreateNewScene(Project *project, char* name);
            void SetActiveScene(char* name);

            Scene& GetActiveScene();
            Scene* GetScenes();
            unsigned int GetSceneCount();
        private:
            void OnEvent(Event event) override;

            Project *current_project;
            unsigned int current_scene_id;
    };
}