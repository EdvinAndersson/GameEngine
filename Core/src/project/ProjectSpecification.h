#pragma once

#include "GLMath.h"

namespace CW {

    enum ResolutionMode {
        WINDOWED,
        FULLSCREEN
    };

    struct ProjectSpecification {
        char *project_name;

        ResolutionMode resolution_mode;
        Vec2 windowed_size;

        bool vsync;
    };

}