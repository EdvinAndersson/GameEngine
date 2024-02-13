#include "AssetsBuilder.h"

namespace CWEditor {

    void AssetsBuilder::Refresh() {
        contents.clear();

        CW::AssetManager& asset_manager = *CW::AssetManager::Get();
        
        auto *loaded_textures = asset_manager.GetLoadedTextures();

        for (auto& it : *loaded_textures) {
            if (it.first == asset_manager.GetDefaultTextureIndex()) continue;
            if (it.first == asset_manager.GetDefaultSpecularTextureIndex()) continue;

            CW::TextureData* texture_data = it.second;

            AssetInfo asset_info = {};
            asset_info.asset_type = AssetType::TEXTURE;
            asset_info.icon = asset_manager.GetDefaultTextureIndex();
            
            char *name = strrchr(texture_data->asset_path_dir, '/');
            if (name != 0)
                asset_info.name = name + 1;
            else
                asset_info.name = texture_data->asset_path_dir;

            contents.insert({texture_data->asset_path_dir, asset_info});
        }
    }
}