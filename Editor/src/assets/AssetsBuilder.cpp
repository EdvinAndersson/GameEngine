#include "AssetsBuilder.h"

namespace CWEditor {

    void AssetsBuilder::Refresh() {
        for (auto content : contents) {
            for (auto asset : content.second.asset_infos)
                delete asset;
        }
        contents.clear();

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

            AssetInfo *asset_info = new AssetInfo();
            asset_info->asset_type = AssetType::TEXTURE;
            asset_info->icon = it.first;
            
            char *name = strrchr(texture_data->asset_path_dir, '/');
            if (name != 0)
                asset_info->name = name + 1;
            else
                asset_info->name = texture_data->asset_path_dir;

            char path[256] = {};
            if (name != 0)
                strncpy(path, texture_data->asset_path_dir, name - texture_data->asset_path_dir);

            size_t path_hash = CW::HashString(path);
            int count = contents_count[path_hash];
            contents[path_hash].asset_infos[count] = asset_info;
            contents_count[path_hash]++;
        }
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
                asset_info->icon = CW::AssetManager::Get()->GetDefaultTextureIndex();
                asset_info->name = next_base_dir;
                strncpy(asset_info->path, next_base_dir, strlen(next_base_dir) - 1);

                size_t dir_hashed = CW::HashString(base_dir);

                if (contents.find(dir_hashed) != contents.end()) {
                    int count = contents_count[dir_hashed];
                    contents[dir_hashed].asset_infos[count] = asset_info;
                    contents_count[dir_hashed]++;

                    printf("DIR 0: %i\n", dir_hashed);
                } else {
                    AssetInfoArray a = {};
                    a.asset_infos[0] = asset_info;
                    contents.insert({dir_hashed, a});
                    contents_count.insert({dir_hashed, 1});

                    printf("DIR 1: %i\n", dir_hashed);
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