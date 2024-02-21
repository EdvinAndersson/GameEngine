#pragma once

#include "cglm/struct.h"
#include "assets/AssetInfo.h"
#include "Core/src/Rendering/Material.h"
#include "Core/src/assets/AssetManager.h"

#include "vendor/imgui/imgui.h"
#include "vendor/imgui/backends/imgui_impl_opengl3.h"
#include "vendor/imgui/backends/imgui_impl_win32.h"

namespace CWEditor {

    inline void UIDragFloat3(char *label, char *text, vec3s *vec3) {
        ImGui::Text(text);
        ImGui::PushItemWidth(-FLT_EPSILON);
        ImGui::DragFloat3(label, (float*) vec3, 0.01f);
        ImGui::PopItemWidth();
    }
    inline void UIAssetList(AssetType asset_type, size_t *asset_array, unsigned int *array_count) {
        char *title = GetDragDropType(asset_type);

        ImGui::Text(title);
        ImGui::SameLine();
        ImGui::PushItemWidth(-FLT_EPSILON);
        char c[64] = {};
        sprintf(c, "%i", asset_array);
        if (ImGui::InputInt(c, (int*) array_count)) {
            *array_count = min(*array_count, MAX_MATERIALS);
        }
        ImGui::PopItemWidth();

        for (unsigned int i = 0; i < *array_count; i++) {
            char buff[256] = {};
            switch (asset_type) {
                case AssetType::MATERIAL: {
                    CW::Material *mat = CW::AssetManager::Get()->GetMaterial(asset_array[i]);
                    if (mat != 0)
                        sprintf(buff, "%s", mat->asset_path);
                } break;
                case AssetType::TEXTURE: {
                    CW::TextureData *tex = CW::AssetManager::Get()->GetTextureData(asset_array[i]);
                    if (tex != 0)
                        sprintf(buff, "%s", tex->asset_path_dir);
                } break;
            }
            char material_label[32] = {};
            sprintf(material_label, "%s %i", title, i);
            
            ImGui::PushItemWidth(-FLT_EPSILON);
            ImGui::InputText(material_label, buff, 128, ImGuiInputTextFlags_ReadOnly);
            ImGui::PopItemWidth();
            if (ImGui::BeginDragDropTarget())
            {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(title))
                {
                    IM_ASSERT(payload->DataSize == sizeof(size_t));
                    size_t index = *(const size_t*)payload->Data;
                    
                    asset_array[i] = index;
                }
                ImGui::EndDragDropTarget();
            }
        }
    }
}