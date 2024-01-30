#pragma once

#include "Core/src/event/IEventListener.h"
#include "Core/src/Cogwheel.h"
#include "Core/src/Rendering/Renderer3D.h"
#include "Core/src/Rendering/Framebuffer.h"
#include "Core/src/Utility.h"

#include "vendor/imgui/imgui.h"
#include "vendor/imgui/backends/imgui_impl_opengl3.h"
#include "vendor/imgui/backends/imgui_impl_win32.h"

#include <cstdlib>

namespace CWEditor {

    class ApplicationView : CW::IEventListener {
        public:
            ApplicationView();
            
            void Init(CW::Cogwheel *cogwheel, CW::Window *window);
            void Update();
        private:
            void OnEvent(CW::Event event) override;
            void RenderDockspace();

            CW::Framebuffer *framebuffer_game_view;
            CW::Cogwheel *cogwheel;
            CW::Window *window;

            float aspect_ratio = 1080 / 1920.0f; // TODO: Create an enum with different aspect ratios
    };
}