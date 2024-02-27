#pragma once

#include "Core/src/Rendering/Texture.h"

namespace CWEditor {

    enum AssetType {
        FOLDER,
        TEXTURE,
        MATERIAL,
        MODEL,
        SCRIPT,
        MESH
    };

    struct AssetInfo {
        AssetType asset_type;
        size_t asset_index;
        char name[256];
        char path[256];
        char drag_drop_type[16];
        CW::TextureIndex icon;
    };

    inline char* GetDragDropType(AssetType asset_type) {
        switch (asset_type)
        {
            case AssetType::MATERIAL: {
                return "Material";
            } break;
            case AssetType::TEXTURE: {
                return "Texture";
            } break;
            case AssetType::SCRIPT: {
                return "Script";
            } break;
            case AssetType::MESH: {
                return "Mesh";
            } break;
        }
        return "";
    }
}