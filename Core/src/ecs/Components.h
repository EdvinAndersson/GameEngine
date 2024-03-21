#pragma once

#include "cglm/struct.h"
#include "Rendering/Model.h"
#include "Rendering/Material.h"

namespace CW {

    const ComponentType BASE_COMPONENTS = 4;

    struct Transform {
        vec3s position;
        vec3s scale;
        vec3s rotation;
        char name[128] = {};
    };
    struct MeshRenderer {
        MeshIndex mesh = 0;
        MaterialIndex materials[MAX_MATERIALS] = {};
        unsigned int material_count = 0;
    };
    struct Light {

    };
    struct Camera {
        bool is_main;
        float fov;
    };
}