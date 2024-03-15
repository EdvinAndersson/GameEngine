#pragma once

#include "cglm/struct.h"

#define PROJECT_NAME_SIZE 128

namespace CW {

    enum ResolutionMode {
        WINDOWED,
        FULLSCREEN
    };

    struct ProjectSpecification {
        char project_name[PROJECT_NAME_SIZE];

        ResolutionMode resolution_mode;
        vec2s windowed_size;

        bool vsync;
    };

}