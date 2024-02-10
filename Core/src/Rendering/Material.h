#pragma once

#include "Texture.h"
#include "cglm/struct.h"

namespace CW {

    using MaterialIndex = size_t;

    struct Material {
        vec3s albedo_color;

        TextureIndex albedo;
        TextureIndex normal_map;
        TextureIndex specular_map;
    };
}