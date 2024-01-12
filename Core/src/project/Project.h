#pragma once
#include "ProjectSpecification.h"

namespace CW {

    struct Project {
        ProjectSpecification specification;
        bool loaded;
    };
}