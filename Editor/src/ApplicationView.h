#pragma once

#include "Core/src/event/IEventListener.h"
#include "Core/src/Cogwheel.h"
#include "Core/src/Rendering/Renderer3D.h"
#include "Core/src/Rendering/Framebuffer.h"
#include "Core/src/Utility.h"

#include "assets/AssetsBuilder.h"

#include "vendor/imgui/imgui.h"
#include "vendor/imgui/backends/imgui_impl_opengl3.h"
#include "vendor/imgui/backends/imgui_impl_win32.h"

#include <cstdlib>

namespace CWEditor {

    class ApplicationView : CW::IEventListener {
        public:
            ApplicationView();
            ~ApplicationView();
            
            void Init(CW::Cogwheel *cogwheel, CW::Window *window);
            void Update();
        private:
            void RenderScene(); 
            void OnEvent(CW::Event event) override;
            void RenderDockspace();
            void RenderAssets();
            void RenderComponents();

            void UIDragFloat3(char *label, char *text, vec3s *vec3);
            void UIAssetList(AssetType asset_type, char *title, size_t *asset_array, unsigned int *array_count);

            bool ShowPopup(CW::GameObject game_object);
            bool CheckNameConflict(char * name);
            char* GetSubDirectory(char *dir);

            CW::Framebuffer *framebuffer_game_view, *framebuffer_dev_view;
            CW::Cogwheel *cogwheel;
            CW::Window *window;
            CW::GameObject selected_game_object;
            
            AssetsBuilder *assets_builder;
            vec2s asset_view_size = { 70.0f, 100.0f };
            size_t current_asset_folder_hash;

            CW::TextureIndex t[8] = {};
            unsigned int count = 0;

            vec3s pos = {0,0,0};
            vec3s dev_pos = {10, 10, 10};
            vec3s cam_rot = {0,0,0};
            vec3s light_pos = {-12.0f, -12.0f, 12.0f};

            CW::TextureData *skybox_texture;
            
            double previous_time = 0;

            vec2s depth_buffer_size = {2048, 2048};
            float aspect_ratio = 1080 / 1920.0f; // TODO: Create an enum with different aspect ratios
    };
}