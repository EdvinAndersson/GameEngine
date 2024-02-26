#include "AssetManager.h"

#include "Rendering/raw_cube.h"
#include "cglm/struct.h"

namespace CW {

    AssetManager *g_asset_manager;

    AssetManager::AssetManager() {
        g_asset_manager = this;

        EventListen(EventType::PROJECT_CREATE);
        EventListen(EventType::PROJECT_LOAD);

        //Load default white texuture
        default_texture_index = HashString("default_texture.png");
        Texture_Format format = { GL_RGBA, TEXTURE_NEAREST_NEIGHBOR };
        TextureData *texture_data = CreateBlankTexture(format, 0xFFFFFFFF);
        strcpy(texture_data->asset_path_dir, "default_texture.png");
        loaded_textures.insert({ default_texture_index, texture_data });

        //Load default specular texuture
        default_specular_texture_index = HashString("default_specular_texture.png");
        Texture_Format format_specular = { GL_RGBA, TEXTURE_NEAREST_NEIGHBOR };
        TextureData *texture_data_specular = CreateBlankTexture(format_specular, 0xFF2F2F2F);
        strcpy(texture_data_specular->asset_path_dir, "default_specular_texture.png");
        loaded_textures.insert({ default_specular_texture_index, texture_data_specular });

        //Load default material
        default_material_index = HashString("default_material.mat");
        Material *material = new Material();
        material->albedo_color = vec3s{ 1.0f, 1.0f, 1.0f };
        material->albedo = default_texture_index;
        material->normal_map = default_texture_index;
        material->specular_map = default_specular_texture_index;
        strcpy(material->asset_path, "default_material.mat");
        loaded_materials.insert({ default_material_index, material });

        //Load default cube mesh
        default_mesh_index = HashString("default_mesh");
        Mesh *mesh = new Mesh();
        mesh->LoadMeshFromData((const char*) g_cube_model, sizeof(g_cube_model));
        loaded_meshes.insert({ default_mesh_index, mesh });
    }

    AssetManager* AssetManager::Get() {
        return g_asset_manager;
    }
    
    void AssetManager::OnEvent(Event e) {
        switch (e.event_type)
        {
            case EventType::PROJECT_CREATE:
            {
                EventData_PROJECT_LOAD *data = (EventData_PROJECT_LOAD *) e.data;

                if (data->project->project_folder_path[0] != '\0') {
                    strcpy(assets_path, data->project->project_folder_path);
                    strcat(assets_path, "/Assets");
                } else {
                    strcpy(assets_path, "Assets");
                }

                if (!FolderExists(assets_path)) {
                    int result = FolderCreate(assets_path);

                    if (result == -1) {
                        printf("Failed to create Assets folder.\n");
                        return;
                    }
                }
            } break;
            case EventType::PROJECT_LOAD:
            {
                EventData_PROJECT_LOAD *data = (EventData_PROJECT_LOAD *) e.data;

                if (data->project->project_folder_path[0] != '\0') {
                    strcpy(assets_path, data->project->project_folder_path);
                    strcat(assets_path, "/Assets");
                } else {
                    strcpy(assets_path, "Assets");
                }

                UnloadAssets();
                LoadAssets();
            } break;
        }
    }
    void AssetManager::LoadAssets() {
        DIR *dir = opendir(assets_path);
        if (dir == 0) {
            printf("Could not open assets folder. %s\n", assets_path);
            return;
        }

        char file_paths[MAX_ASSETS][256] = {};
        unsigned int file_count = 0;
        char base_dir[256] = "";
        GetAllAssetPaths(dir, file_paths, &file_count, base_dir);

        for (unsigned int i = 0; i < file_count; i++) {
            char *file_type = strchr(file_paths[i], '.');

            if (file_type == 0) //if no extention, skip
                continue;
            file_type++;

            printf ("Asset: %s, %s\n", file_paths[i], file_type);

            if (strcmp(file_type, "png") == 0 || strcmp(file_type, "jpg") == 0) {
                LoadTexture(file_paths[i]);
                continue;
            }
            if (strcmp(file_type, "mat") == 0) {
                LoadMaterial(file_paths[i]);
                continue;
            }
            if (strcmp(file_type, "obj") == 0) {
                LoadModel(file_paths[i]);
                continue;
            }
            if (strcmp(file_type, "h") == 0) {
                LoadScript(file_paths[i]);
                continue;
            }
        }
    }
    void AssetManager::UnloadAssets() {
        //Unload textures
        for (auto& it : loaded_textures) {
            if (it.first == default_texture_index || it.first == default_specular_texture_index)
                continue;
            FreeTextureData(it.second);
            delete it.second;
        }
        CW::TextureData *default_texture_data = loaded_textures[default_texture_index];
        CW::TextureData *default_specular_texture_data = loaded_textures[default_specular_texture_index]; 
        loaded_textures.clear();
        loaded_textures.insert({ default_texture_index, default_texture_data });
        loaded_textures.insert({ default_specular_texture_index, default_specular_texture_data });

        //Unload materials
        for (auto& it : loaded_materials) {
            if (it.first == default_material_index)
                continue;
            delete it.second;
        }
        CW::Material *default_material = loaded_materials[default_material_index];
        loaded_materials.clear();
        loaded_materials.insert({ default_material_index, default_material });

        //Unload meshes
        for (auto& it : loaded_meshes) {
            if (it.first == default_mesh_index)
                continue;
            delete it.second;
        }
        Mesh *default_mesh = loaded_meshes[default_mesh_index];
        loaded_meshes.clear();
        loaded_meshes.insert({ default_mesh_index, default_mesh });

        //Unload models
        for (auto& it : loaded_models) {
            delete it.second;
        }
        loaded_models.clear();

        //Unload scripts
        for (auto& it : loaded_scripts) {
            delete it.second;
        }
        loaded_scripts.clear();
    }

    void AssetManager::LoadTexture(char *path) {
        size_t hashed_path = HashString(path);

        if (loaded_textures.find(hashed_path) != loaded_textures.end()){
            printf("Texture is already loaded! %s\n", path);
            return;
        }
        char full_path[1024] = {};
        strcpy(full_path, assets_path);
        strcat(full_path, "/");
        strcat(full_path, path);

        TextureData *texture_data = CreateTexture(full_path, path);
        loaded_textures.insert({ hashed_path, texture_data });
    }

    Texture AssetManager::GetTexture(char *path) {
        size_t hashed_path = HashString(path);

        return loaded_textures[hashed_path]->texture;
    }

    TextureIndex AssetManager::GetTextureIndex(char *path) {
        return HashString(path);
    }

    void AssetManager::LoadMaterial(char *path) {
        size_t hashed_path = HashString(path);

        if (loaded_materials.find(hashed_path) != loaded_materials.end()){
            printf("Material is already loaded! %s\n", path);
            return;
        }
        char full_path[1024] = {};
        strcpy(full_path, assets_path);
        strcat(full_path, "/");
        strcat(full_path, path);

        FILE *file = fopen(full_path, "r");
        if (!file) {
            printf("Falied to load material: %s\n", full_path);
            return;
        }

        char data[MATERIAL_ASSET_FILE_SIZE];
        fread(data, 1, MATERIAL_ASSET_FILE_SIZE, file);

        Deserialize deserialize(data, ';');
        Material *material = new Material();
        material->albedo_color = deserialize.GetVec3();
        material->albedo = deserialize.GetSize_t();
        material->normal_map = deserialize.GetSize_t();
        material->specular_map = deserialize.GetSize_t();
        strcpy(material->asset_path, path);

        loaded_materials.insert({ hashed_path, material });

        fclose(file);
    }

    void AssetManager::CreateAndLoadMaterialAsset(char *path, Material material) {
        char full_path[1024] = {};
        strcpy(full_path, assets_path);
        strcat(full_path, "/");
        strcat(full_path, path);

        FILE *file = fopen(full_path, "w");
        if (!file) {
            printf("Falied to create material asset. %s\n", path);
            return;
        }
        Serialize serialize(MATERIAL_ASSET_FILE_SIZE, ';');
        serialize.SubmitVec3(material.albedo_color);
        serialize.SubmitSize_t(material.albedo);
        serialize.SubmitSize_t(material.normal_map);
        serialize.SubmitSize_t(material.specular_map);

        fwrite(serialize.GetData(), 1, strlen(serialize.GetData()), file);

        fclose(file);

        size_t hashed_path = HashString(path);
        Material *mat = new Material();
        mat->albedo_color = material.albedo_color;
        mat->albedo = material.albedo;
        mat->normal_map = material.normal_map;
        strcpy(mat->asset_path, path);

        loaded_materials.insert({ hashed_path, mat });
    }

    Material* AssetManager::GetMaterial(MaterialIndex material_index) {
        return loaded_materials[material_index];
    }

    MaterialIndex AssetManager::GetMaterialIndex(char *path) {
        return HashString(path);
    }

    void AssetManager::LoadMesh(char *path) {
        size_t hashed_path = HashString(path);

        if (loaded_meshes.find(hashed_path) != loaded_meshes.end()){
            printf("Mesh is already loaded! %s\n", path);
            return;
        }
        char full_path[1024] = {};
        strcpy(full_path, assets_path);
        strcat(full_path, "/");
        strcat(full_path, path);

        //Mesh *mesh = new Mesh();
        //mesh.L
    }

    Mesh* AssetManager::GetMesh(MeshIndex mesh_index) {
        return loaded_meshes[mesh_index];
    }

    void AssetManager::LoadModel(char *path) {
        size_t hashed_path = HashString(path);

        if (loaded_models.find(hashed_path) != loaded_models.end()){
            printf("Model is already loaded! %s\n", path);
            return;
        }
        char full_path[1024] = {};
        strcpy(full_path, assets_path);
        strcat(full_path, "/");
        strcat(full_path, path);

        Model *model = new Model();
        model->Load(full_path, path);
        loaded_models.insert({ hashed_path, model });

        Mesh *mesh = new Mesh();
        mesh->Load(model->GetAiScene());
        loaded_meshes.insert({ hashed_path, mesh });

        model->mesh_index = hashed_path;
    }

    void AssetManager::LoadScript(char *path) {
        size_t hashed_path = HashString(path);

        if (loaded_scripts.find(hashed_path) != loaded_scripts.end()){
            printf("Script is already loaded! %s\n", path);
            return;
        }

        ScriptData *script_data = new ScriptData();
        strcpy(script_data->asset_path, path);
        char *name = strchr(path, '/') + 1;
        strncpy(script_data->name, name, strlen(name)-2); //remove .h
        printf("name: %s\n", script_data->name);

        loaded_scripts.insert({ hashed_path, script_data });
    }

    void AssetManager::GetAllAssetPaths(DIR *dir, char file_paths[MAX_ASSETS][256], unsigned int *count, char base_dir[256]) {
        struct dirent *ent;

        while ((ent = readdir (dir)) != NULL) {
            if (ent->d_type == DT_DIR && (strcmp(ent->d_name,".") == 0 || strcmp(ent->d_name,"..") == 0))
                continue;

            if (ent->d_type == DT_DIR) {

                char next_base_dir[256];
                strcpy(next_base_dir, base_dir);
                strcat(next_base_dir, ent->d_name);
                strcat(next_base_dir, "/");

                char new_path[256] = {};
                strcpy(new_path, assets_path);
                strcat(new_path, "/");
                strcat(new_path, next_base_dir);

                DIR *di = opendir(new_path);
                GetAllAssetPaths(di, file_paths, count, next_base_dir);
            } else {
                char new_path[256] = {};
                strcpy(new_path, base_dir);
                strcat(new_path, ent->d_name);
                strcpy(file_paths[(*count)++], new_path);
            }

        }
        closedir(dir);
    }
}