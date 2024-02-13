#pragma once

#include "Texture.h"

namespace CWEditor {

    enum AssetType {
        TEXTURE,
        MATERIAL,
        MODEL
    };

    struct Asset {
        AssetType asset_type;
        char *name;
        CW::TextureIndex icon;
    };
}