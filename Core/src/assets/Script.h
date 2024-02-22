#pragma once

#include "ecs/Component.h"

namespace CW {

    using ScriptIndex = size_t;

    struct ScriptData {
        char asset_path[256];
        char name[256];
        ComponentType type;
    };
}