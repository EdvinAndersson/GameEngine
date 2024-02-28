#pragma once

#include "cglm/struct.h"
#include "assets/AssetInfo.h"
#include "Core/src/Rendering/Material.h"
#include "Core/src/assets/AssetManager.h"

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "imgui/backends/imgui_impl_win32.h"

#include "Core/src/assets/ScriptManager.h"

namespace CWEditor {

    inline void UIDragFloat3(char *label, char *text, vec3s *vec3) {
        ImGui::Text(text);
        ImGui::PushItemWidth(-FLT_EPSILON);
        ImGui::DragFloat3(label, (float*) vec3, 0.01f);
        ImGui::PopItemWidth();
    }
    inline void UIAssetInput(AssetType asset_type, size_t *asset_array) {
        char *title = GetDragDropType(asset_type);

        ImGui::Text(title);
        ImGui::SameLine();
        
        char buff[256] = {};
        switch (asset_type) {
            case AssetType::MATERIAL: {
                CW::Material *mat = CW::AssetManager::Get()->GetMaterial(*asset_array);
                if (mat != 0)
                    strcpy(buff, mat->asset_path);
            } break;
            case AssetType::TEXTURE: {
                CW::TextureData *tex = CW::AssetManager::Get()->GetTextureData(*asset_array);
                if (tex != 0)
                    strcpy(buff, tex->asset_path_dir);
            } break;
            case AssetType::MESH: {
                CW::Mesh *mesh = CW::AssetManager::Get()->GetMesh(*asset_array);
                if (mesh != 0)
                    strcpy(buff, mesh->asset_path);
            } break;
        }
        
        ImGui::PushItemWidth(-FLT_EPSILON);
        ImGui::InputText(title, buff, 128, ImGuiInputTextFlags_ReadOnly);
        ImGui::PopItemWidth();
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(title))
            {
                IM_ASSERT(payload->DataSize == sizeof(size_t));
                size_t index = *(const size_t*)payload->Data;
                
                *asset_array = index;
            }
            ImGui::EndDragDropTarget();
        }
    }
    inline void UIAssetInputList(AssetType asset_type, size_t *asset_array, unsigned int *array_count) {
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
                case AssetType::MESH: {
                    CW::Mesh *mesh = CW::AssetManager::Get()->GetMesh(asset_array[i]);
                    if (mesh != 0)
                        sprintf(buff, "%s", mesh->asset_path);
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
    inline void RenderAssetPopup(CW::GameObject game_object, AssetType asset_type){
        char title[128] = "Asset Popup";
        ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize; 
        if (ImGui::BeginPopup(title, flags)){
            static ImGuiTextFilter filter;
            ImGui::Text("Search");
            ImGui::PushItemWidth(-FLT_EPSILON);
            filter.Draw("Search", 200);
            ImGui::PopItemWidth();

            switch (asset_type) {
                case AssetType::SCRIPT : {
                    if (!game_object.HasComponent<CW::MeshRenderer>() && filter.PassFilter("MeshRenderer")) {
                        if (ImGui::Button("MeshRenderer")){
                            CW::MeshRenderer& meshtemp = game_object.AddComponent<CW::MeshRenderer>();
                            meshtemp.mesh = CW::AssetManager::Get()->GetDefaultMeshIndex();
                            meshtemp.materials[0] = CW::AssetManager::Get()->GetDefaultMaterialIndex();
                            meshtemp.material_count = 1;
                            ImGui::CloseCurrentPopup();
                        }
                    }

                    auto *loaded_scripts = CW::AssetManager::Get()->GetLoadedScripts();
                    for (auto& it : *loaded_scripts) {
                        CW::ScriptData* script_data = it.second;
                        size_t hashed = CW::HashString(script_data->name);
                        if (CW::HasGenereatedComponent(hashed, game_object)) continue;

                        if (filter.PassFilter(script_data->name) && ImGui::Button(script_data->name)) {
                            CW::AddGenereatedComponent(hashed, game_object);
                            ImGui::CloseCurrentPopup();
                        }
                    }
                } break;
                case AssetType::TEXTURE: {

                } break;
            }
            ImGui::EndPopup();    
        }
    }
    inline void OpenAssetPopup(){
        char title[128] = "Asset Popup"; 
        ImGui::OpenPopup(title);   
    }
}