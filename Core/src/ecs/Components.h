#pragma once

#include "Utility/GLMath.h"
#include "Utility/Rendering/Model.h"

namespace CW {

    struct Transform {
        Vec3 position;
        Vec3 scale;
        Vec3 rotation;
    };
    struct ModelRenderer {
        Model *model;
        Vec3 tint = { 1.0f, 1.0f, 1.0f };
    };
}