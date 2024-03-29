#pragma once

#include <stdio.h>

#include "Core/src/Cogwheel.h"
#include "Core/src/event/EventManager.h"
#include "Core/src/event/IEventListener.h"
#include "Core/src/ecs/GameObject.h"
#include "Core/src/Rendering/Renderer3D.h"

#include "ApplicationView.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace CWEditor {

    enum PlayMode {
        NOT_RUNNING,
        RUNNING,
        RUNNING_PAUSED
    };

    class Application : public CW::IEventListener {
        public:
            ~Application();

            void Init();
            void Run();

            void OnEvent(CW::Event event) override;
        private:
            CW::Cogwheel *cogwheel;
            CW::Window *window;
            ApplicationView *application_view;

            PlayMode play_mode;
    };
}