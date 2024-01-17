#pragma once

#include "../ecs/GameObject.h"

#include <set>

namespace CW {
    
    #define MAX_SCENES 16

    struct Scene {
        char name[1024];
        std::set<GameObject> game_objects;
    };
}