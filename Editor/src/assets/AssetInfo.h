#pragma once

#include "Core/src/Rendering/Texture.h"

namespace CWEditor {

    enum AssetType {
        FOLDER,
        TEXTURE,
        MATERIAL,
        MODEL
    };

    struct AssetInfo {
        AssetType asset_type;
        char name[256];
        char path[256];
        CW::TextureIndex icon;
    };
}