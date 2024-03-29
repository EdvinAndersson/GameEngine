#pragma once

#include "Core/src/event/IEventListener.h"
#include "Core/src/Cogwheel.h"
#include "Core/src/Rendering/Renderer3D.h"
#include "Core/src/Rendering/Framebuffer.h"
#include "Core/src/Utility.h"
#include "Core/src/project/ProjectSpecification.h"

#include "tools/GeneratedComponentsUtility.h"

#include "assets/AssetsBuilder.h"
#include "ImGuiComponents.h"
#include "Console.h"
#include "ImGuiStyles.h"

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "imgui/backends/imgui_impl_win32.h"

#include <cstdlib>
#include <shobjidl.h>

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
            void RenderAssetInspector();
            void RenderProjectSettings();
            bool RenderAssetCreationPopup();

            inline void ShowProjectSettings(bool show) { show_project_settings = show; }
            bool ShowPopup(CW::GameObject game_object);
            bool CheckNameConflict(char * name);
            char* GetSubDirectory(char *dir);
            std::string GetProjectFilePath();

            CW::Framebuffer *framebuffer_game_view, *framebuffer_dev_view;
            CW::Cogwheel *cogwheel;
            CW::Window *window;
            Console *console;

            CW::GameObject selected_game_object = CW::GameObject {};
            AssetInfo selected_asset = {};
            
            //Assets View
            AssetsBuilder *assets_builder;
            vec2s asset_view_size = { 80.0f, 110.0f };
            size_t current_asset_folder_hash;
            AssetInfo *selected_asset_info;

            bool show_project_settings = false;

            CW::TextureIndex t[8] = {};
            unsigned int count = 0;

            vec3s pos = {0,0,0};
            vec3s dev_pos = {0,0,0};
            vec3s cam_rot = {0,0,0};
            vec3s light_pos = {2.0f, 5.0f, 2.0f};

            CW::TextureData *skybox_texture;
            
            double previous_time = 0;

            vec2s depth_buffer_size = {2048, 2048};
            float aspect_ratio = 1080 / 1920.0f; // TODO: Create an enum with different aspect ratios
    };
}