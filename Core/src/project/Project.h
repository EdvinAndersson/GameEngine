#pragma once

#include "ProjectSpecification.h"
#include "scene/Scene.h"

namespace CW {

    struct Project {
        ProjectSpecification specification;
        char project_folder_path[1024];

        Scene scenes[MAX_SCENES];
        unsigned int scene_count;
    };
}