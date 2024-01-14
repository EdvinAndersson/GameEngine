#pragma once

#include <stdio.h>

#include "Core/src/Cogwheel.h"
#include "Core/src/event/EventManager.h"
#include "Core/src/event/IEventListener.h"
#include "Core/src/ecs/GameObject.h"

#include "Utility/Rendering/Renderer3D.h"

#include "ApplicationView.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace CWEditor {

    class Application : public CW::IEventListener {
        public:
            ~Application();

            void Init();
            void Run();

            void OnEvent(CW::Event event) override;
        private:
            CW::Cogwheel *cogwheel;
            Window *window;
            ApplicationView *application_view;
    };
}