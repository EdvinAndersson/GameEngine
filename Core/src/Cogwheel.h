#pragma once

#include "Window.h"
#include "event/IEventListener.h"
#include "ecs/ECS.h"
#include "SceneManager.h"
#include "project/ProjectManager.h"

namespace CW {

    class Cogwheel : public IEventListener {

        public:
            Cogwheel();
            ~Cogwheel();

            int Init();
            void Update();
            void Stop();
            void SwapBuffersAndPollEvents();
            inline bool IsRunning() { return running; }
            inline Window *GetWindow() { return window; }
            inline SceneManager *GetSceneManager() { return scene_manager; }
            inline ProjectManager *GetProjectManager() { return project_manager; }

            void OnEvent(Event event) override;
        private:
            CW::Window *window;
            CW::ECS *ecs;
            CW::SceneManager *scene_manager;
            CW::ProjectManager *project_manager;
            bool running;
    };
}