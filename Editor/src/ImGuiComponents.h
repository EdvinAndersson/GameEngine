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

    inline size_t RenderAssetPopup(CW::GameObject game_object, AssetType asset_type);
    inline void OpenAssetPopup();

    inline bool UIDragFloat3(char *label, char *text, vec3s *vec3) {
        bool updated = false;

        ImGui::Text(text);
        ImGui::PushItemWidth(-FLT_EPSILON);
        updated = ImGui::DragFloat3(label, (float*) vec3, 0.02f);
        ImGui::PopItemWidth();

        return updated;
    }
    inline bool UICheckbox(char *text, bool *value) {
        bool updated = false;

        ImGui::Text(text);
        ImGui::SameLine();
        updated = ImGui::Checkbox("##On", value);

        return updated;
    }
    inline bool UIAssetInput(AssetType asset_type, char *label, size_t *asset) {
        ImGui::PushID(asset);
        bool updated = false;
        char *asset_type_name = GetDragDropType(asset_type);

        ImGui::Text(label);
        ImGui::SameLine();
        
        char buff[256] = {};

        if (*asset != 0) {
            switch (asset_type) {
                case AssetType::MATERIAL: {
                    CW::Material *mat = CW::AssetManager::Get()->GetMaterial(*asset);
                    if (mat != 0)
                        strcpy(buff, mat->asset_path);
                } break;
                case AssetType::TEXTURE: {
                    CW::TextureData *tex = CW::AssetManager::Get()->GetTextureData(*asset);
                    if (tex != 0)
                        strcpy(buff, tex->asset_path_dir);
                } break;
                case AssetType::MESH: {
                    CW::Mesh *mesh = CW::AssetManager::Get()->GetMesh(*asset);
                    if (mesh != 0)
                        strcpy(buff, mesh->asset_path);
                } break;
            }
        }
        
        size_t asset_index = RenderAssetPopup(CW::GameObject {}, asset_type);
        if (asset_index != 0)
            *asset = asset_index;

        ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0,0));
        if (ImGui::Button(buff, ImVec2(ImGui::GetWindowSize().x, 0.0f))){
            OpenAssetPopup();
        }
        
        ImGui::PopStyleVar();
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(asset_type_name)) {
                updated = true;
                IM_ASSERT(payload->DataSize == sizeof(size_t));
                size_t index = *(const size_t*)payload->Data;
                
                *asset = index;
            }
            ImGui::EndDragDropTarget();
        }
        ImGui::PopID();

        return updated;
    }
    inline void UIAssetInputList(AssetType asset_type, size_t *asset_array, unsigned int *array_count) {
        ImGui::PushID(asset_array);
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
            ImGui::PushID(i);
            UIAssetInput(asset_type, "", &asset_array[i]);
            ImGui::PopID();
        }
        ImGui::PopID();
    }
    inline size_t RenderAssetPopup(CW::GameObject game_object, AssetType asset_type) {
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
                        if (CW::HasGeneratedComponent(hashed, game_object)) continue;

                        if (filter.PassFilter(script_data->name) && ImGui::Button(script_data->name)) {
                            CW::AddGeneratedComponent(hashed, game_object);
                            ImGui::CloseCurrentPopup();
                        }
                    }
                } break;
                case AssetType::TEXTURE: {
                    auto *loaded_textures = CW::AssetManager::Get()->GetLoadedTextures();
                    for (auto& it : *loaded_textures) {
                        CW::TextureData* texture_data = it.second;
                        if (filter.PassFilter(texture_data->asset_path_dir) && ImGui::Button(texture_data->asset_path_dir)) {
                            ImGui::CloseCurrentPopup();
                            ImGui::EndPopup();
                            return it.first;
                        }
                    }
                } break;
                case AssetType::MATERIAL: {
                    auto *loaded_materials = CW::AssetManager::Get()->GetLoadedMaterials();
                    for (auto& it : *loaded_materials) {
                        CW::Material* material = it.second;
                        if (filter.PassFilter(material->asset_path) && ImGui::Button(material->asset_path)) {
                            ImGui::CloseCurrentPopup();
                            ImGui::EndPopup();
                            return it.first;
                        }
                    }
                } break;
                case AssetType::MESH: {
                    auto *loaded_meshes = CW::AssetManager::Get()->GetLoadedMeshes();
                    for (auto& it : *loaded_meshes) {
                        CW::Mesh* mesh = it.second;
                        if (filter.PassFilter(mesh->asset_path) && ImGui::Button(mesh->asset_path)) {
                            ImGui::CloseCurrentPopup();
                            ImGui::EndPopup();
                            return it.first;
                        }
                    }
                } break;
            }
            ImGui::EndPopup();    
        }
        return 0;
    }
    inline void OpenAssetPopup() {
        char title[128] = "Asset Popup"; 
        ImGui::OpenPopup(title);   
    }
}