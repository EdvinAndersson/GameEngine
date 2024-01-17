#pragma once
#include "ProjectSpecification.h"
#include "../scene/Scene.h"

namespace CW {

    struct Project {
        ProjectSpecification specification;
        bool loaded;

        Scene scenes[MAX_SCENES];
        unsigned int scene_count;
    };
}