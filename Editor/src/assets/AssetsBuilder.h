#pragma once

#include "Core/src/assets/AssetManager.h"
#include "AssetInfo.h"

namespace CWEditor {

    struct AssetInfoArray {
        AssetInfo asset_infos[MAX_ASSETS];
    };

    class AssetsBuilder {
        public:
            AssetsBuilder() {}

            void Refresh();

            inline std::unordered_map<char *, AssetInfoArray>* GetContents() { return &contents; }
        private:
            void MakeAssetsFolders(DIR *dir, char *base_dir);

            std::unordered_map<char *, AssetInfoArray> contents;
            std::unordered_map<char *, int> contents_count;
    };
}