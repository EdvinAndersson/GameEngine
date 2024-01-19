#pragma once

#include "event/IEventListener.h"
#include "ecs/ECS.h"
#include "scene/SceneManager.h"
#include "project/ProjectManager.h"
#include "assets/AssetManager.h"

namespace CW {

    class Cogwheel : public IEventListener {

        public:
            Cogwheel();
            ~Cogwheel();

            int Init();
            void Update();
            void Stop();
            inline bool IsRunning() { return running; }
            inline SceneManager *GetSceneManager() { return scene_manager; }
            inline ProjectManager *GetProjectManager() { return project_manager; }

            void OnEvent(Event event) override;
        private:
            ECS *ecs;
            SceneManager *scene_manager;
            ProjectManager *project_manager;
            AssetManager *asset_manager;
            bool running;
    };
}