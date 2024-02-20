#include "AssetsBuilder.h"

namespace CWEditor {

    void AssetsBuilder::Refresh() {
        for (auto content : contents) {
            for (auto asset : content.second.asset_infos)
                delete asset;
        }
        contents.clear();
        contents_count.clear();

        CW::AssetManager& asset_manager = *CW::AssetManager::Get();
        char *assets_path = asset_manager.GetAssetsPath();

        DIR *dir = opendir(assets_path);
        CW_ASSERT(dir != 0, "Could not open assets folder!");

        MakeAssetsFolders(dir, "");

        auto *loaded_textures = asset_manager.GetLoadedTextures();
        for (auto& it : *loaded_textures) {
            if (it.first == asset_manager.GetDefaultTextureIndex()) continue;
            if (it.first == asset_manager.GetDefaultSpecularTextureIndex()) continue;

            CW::TextureData* texture_data = it.second;

            CreateAssetInfo(AssetType::TEXTURE, it.first, it.first, texture_data->asset_path_dir);
        }
        auto loaded_materials = asset_manager.GetLoadedMaterials();

        for (auto& it : *loaded_materials) {
            if (it.first == asset_manager.GetDefaultMaterialIndex()) continue;

            CW::Material* material = it.second;

            CreateAssetInfo(AssetType::MATERIAL, it.first, asset_manager.GetDefaultTextureIndex(), material->asset_path);
        }
    }
    void AssetsBuilder::CreateAssetInfo(AssetType asset_type, size_t asset_index, CW::TextureIndex texture_index, char *asset_path) {
        AssetInfo *asset_info = new AssetInfo();
        asset_info->asset_type = asset_type;
        asset_info->asset_index = asset_index;
        asset_info->icon = texture_index;
        
        char *name = strrchr(asset_path, '/');
        if (name != 0) 
            strcpy(asset_info->name, name+1);
        else
            strcpy(asset_info->name, asset_path);

        char path[256] = {};
        if (name != 0)
            strncpy(path, asset_path, name - asset_path);

        size_t path_hash = CW::HashString(path);
        int count = contents_count[path_hash];
        contents[path_hash].asset_infos[count] = asset_info;
        contents_count[path_hash]++;
    }
    void AssetsBuilder::MakeAssetsFolders(DIR *dir, char *base_dir) {
        struct dirent *ent;

        while ((ent = readdir(dir)) != NULL) {
            if (ent->d_type == DT_DIR && (strcmp(ent->d_name,".") == 0 || strcmp(ent->d_name,"..") == 0))
                continue;

            if (ent->d_type == DT_DIR) {
                char next_base_dir[256] = {};
                strcpy(next_base_dir, base_dir);
                strcat(next_base_dir, ent->d_name);
                strcat(next_base_dir, "/");

                AssetInfo *asset_info = new AssetInfo();
                asset_info->asset_type = AssetType::FOLDER;
                asset_info->icon = CW::AssetManager::Get()->GetTextureIndex("images/folder_icon.png");
                strncpy(asset_info->path, next_base_dir, strlen(next_base_dir) - 1);
                strcpy(asset_info->name, asset_info->path);
                
                char next_base_without_slash[256] = {};
                if (strchr(base_dir, '/') != 0) {
                    strncpy(next_base_without_slash, base_dir, strlen(base_dir) - 1);
                }
                size_t dir_hashed = CW::HashString(next_base_without_slash);

                if (contents.find(dir_hashed) != contents.end()) {
                    int count = contents_count[dir_hashed];
                    contents[dir_hashed].asset_infos[count] = asset_info;
                    contents_count[dir_hashed]++;
                } else {
                    AssetInfoArray a = {};
                    a.asset_infos[0] = asset_info;
                    contents.insert({dir_hashed, a});
                    contents_count.insert({dir_hashed, 1});
                }

                char new_path[256] = {};
                strcpy(new_path, CW::AssetManager::Get()->GetAssetsPath());
                strcat(new_path, "/");
                strcat(new_path, next_base_dir);

                DIR *di = opendir(new_path);
                CW_ASSERT(di != 0, "Could not open assets folder!");
                MakeAssetsFolders(di, next_base_dir);
            }
        }
        closedir(dir);
    }
}