#pragma once

#include "GLMath.h"
#include "Rendering/Model.h"
#include "Rendering/Material.h"

namespace CW {

    struct Transform {
        Vec3 position;
        Vec3 scale;
        Vec3 rotation;
    };
    struct MeshRenderer {
        MeshIndex mesh;
        MaterialIndex material;
    };
}