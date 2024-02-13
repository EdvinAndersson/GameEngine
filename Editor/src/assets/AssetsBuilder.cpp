#include "AssetsBuilder.h"

namespace CWEditor {

    void AssetsBuilder::Refresh() {
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

            AssetInfo asset_info = {};
            asset_info.asset_type = AssetType::TEXTURE;
            asset_info.icon = it.first;
            
            char *name = strrchr(texture_data->asset_path_dir, '/');
            if (name != 0)
                asset_info.name = name + 1;
            else
                asset_info.name = texture_data->asset_path_dir;

            //contents.insert({texture_data->asset_path_dir, asset_info});
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

                char new_path[256] = {};
                strcpy(new_path, CW::AssetManager::Get()->GetAssetsPath());
                strcat(new_path, "/");
                strcat(new_path, next_base_dir);

                AssetInfo asset_info = {};
                asset_info.asset_type = AssetType::FOLDER;
                asset_info.icon = CW::AssetManager::Get()->GetDefaultTextureIndex();
                asset_info.name = next_base_dir;

                if (contents.find(asset_info.name) != contents.end()) {
                    int count = contents_count[asset_info.name];
                    contents[asset_info.name].asset_infos[count] = asset_info;
                    contents_count[asset_info.name]++;
                } else {
                    AssetInfoArray a = {};
                    a.asset_infos[0] = asset_info;
                    contents.insert({asset_info.name, a});
                    contents_count.insert({asset_info.name, 1});
                }

                DIR *di = opendir(new_path);
                MakeAssetsFolders(di, next_base_dir);
            }
        }
        closedir(dir);
    }
}