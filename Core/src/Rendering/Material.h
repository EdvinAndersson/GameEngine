#pragma once

#include "Texture.h"
#include "GLMath.h"

namespace CW {

    using MaterialIndex = size_t;

    struct Material {
        Vec3 albedo_color;

        TextureIndex albedo;
        TextureIndex normal_map;
    };
}