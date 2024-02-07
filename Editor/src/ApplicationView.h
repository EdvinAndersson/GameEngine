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
            void RenderScene(); 
            void OnEvent(CW::Event event) override;
            void RenderDockspace();

            CW::Framebuffer *framebuffer_game_view;
            CW::Cogwheel *cogwheel;
            CW::Window *window;

            vec3s pos = {0,0,0};
            vec3s light_pos = {-12.0f, -12.0f, 12.0f};

            CW::TextureData *skybox_texture;
            
            double previous_time = 0;

            vec2s depth_buffer_size = {2048, 2048};
            float aspect_ratio = 1080 / 1920.0f; // TODO: Create an enum with different aspect ratios
    };
}