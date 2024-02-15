#pragma once

#include <unordered_map>

#include "Rendering/Texture.h"
#include "Rendering/Model.h"
#include "Rendering/Material.h"
#include "Utility.h"
#include "serialization/Deserialize.h"
#include "serialization/Serialize.h"
#include "event/IEventListener.h"
#include "vendor/dirent/dirent.h"

namespace CW {

    #define MAX_ASSETS 1024

    #define MATERIAL_ASSET_FILE_SIZE 1024

    class AssetManager : IEventListener {
        public:
            AssetManager();

            void LoadAssets();
            void UnloadAssets();
            void OnEvent(Event e) override;

            void LoadTexture(char *path);
            Texture GetTexture(char *path);
            TextureIndex GetTextureIndex(char *path);
            inline Texture GetTexture(TextureIndex texture) { return loaded_textures[texture]->texture; }
            inline TextureData* GetTextureData(TextureIndex texture) { return loaded_textures[texture]; }

            void LoadMaterial(char *path);
            void CreateAndLoadMaterialAsset(char *path, Material material);
            Material* GetMaterial(MaterialIndex material_index);
            MaterialIndex GetMaterialIndex(char *path);

            void LoadMesh(char *path);
            Mesh* GetMesh(MeshIndex mesh_index);
            inline MeshIndex GetMeshIndex(char *path) { return HashString(path); }

            void LoadModel(char *path);
            inline ModelIndex GetModelIndex(char *path) { return HashString(path); }
            Model* GetModel(ModelIndex model_index) { return loaded_models[model_index]; }

            inline TextureIndex GetDefaultTextureIndex() { return default_texture_index; }
            inline TextureIndex GetDefaultSpecularTextureIndex() { return default_specular_texture_index; }
            inline MaterialIndex GetDefaultMaterialIndex() { return default_material_index; }
            inline MeshIndex GetDefaultMeshIndex() { return default_mesh_index; }

            inline std::unordered_map<TextureIndex, TextureData *>* GetLoadedTextures() { return &loaded_textures; }
            inline std::unordered_map<MaterialIndex, Material *>* GetLoadedMaterials() { return &loaded_materials; }

            inline char* GetAssetsPath() { return assets_path; }

            static AssetManager* Get();
        private:
            void GetAllAssetPaths(DIR *dir, char file_paths[MAX_ASSETS][256], unsigned int *count, char base_dir[256]);

            std::unordered_map<TextureIndex, TextureData *> loaded_textures = {};
            std::unordered_map<MaterialIndex, Material *> loaded_materials = {};

            std::unordered_map<ModelIndex, Model *> loaded_models = {};
            std::unordered_map<MeshIndex, Mesh *> loaded_meshes = {};

            TextureIndex default_texture_index, default_specular_texture_index;
            MaterialIndex default_material_index;
            MeshIndex default_mesh_index;
            char assets_path[1024];
    };
}