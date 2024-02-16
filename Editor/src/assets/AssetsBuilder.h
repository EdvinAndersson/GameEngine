#pragma once

#include "Core/src/assets/AssetManager.h"
#include "AssetInfo.h"

namespace CWEditor {

    struct AssetInfoArray {
        AssetInfo *asset_infos[MAX_ASSETS];
    };

    class AssetsBuilder {
        public:
            AssetsBuilder() {}

            void Refresh();

            inline std::unordered_map<size_t, AssetInfoArray>* GetContents() { return &contents; }
            inline int GetContentsCount(size_t dir_hash) { return contents_count[dir_hash]; }
        private:
            void CreateAssetInfo(AssetType asset_type, size_t asset_index, CW::TextureIndex texture_index, char *asset_path);
            void MakeAssetsFolders(DIR *dir, char *base_dir);

            std::unordered_map<size_t, AssetInfoArray> contents;
            std::unordered_map<size_t, int> contents_count;
    };
}