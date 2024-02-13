#pragma once

#include "Core/src/assets/AssetManager.h"
#include "AssetInfo.h"

namespace CWEditor {

    class AssetsBuilder {
        public:
            AssetsBuilder() {}

            void Refresh();

            inline std::unordered_map<char *, AssetInfo>* GetContents() { return &contents; }
        private:
            std::unordered_map<char *, AssetInfo> contents;
    };
}