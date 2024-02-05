#pragma once

#include "cglm/struct.h"

namespace CW {

    enum ResolutionMode {
        WINDOWED,
        FULLSCREEN
    };

    struct ProjectSpecification {
        char *project_name;

        ResolutionMode resolution_mode;
        vec2s windowed_size;

        bool vsync;
    };

}