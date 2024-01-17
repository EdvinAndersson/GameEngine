#pragma once

#include "event/IEventListener.h"
#include "ecs/ECS.h"
#include "scene/SceneManager.h"
#include "project/ProjectManager.h"

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
            CW::ECS *ecs;
            CW::SceneManager *scene_manager;
            CW::ProjectManager *project_manager;
            bool running;
    };
}