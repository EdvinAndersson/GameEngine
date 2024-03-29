#pragma once

#include "Window.h"
#include "ecs/GameObject.h"
#include "project/Project.h"

namespace CW {

    enum EventType {
        WINDOW_CLOSE,
        WINDOW_RESIZE,
        WINDOW_KEYDOWN,
        WINDOW_KEYUP,
        WINDOW_KEYPRESSED,

        PROJECT_LOAD,
        PROJECT_CREATE,
        PROJECT_LOAD_LATE,

        ECS_INSTANTIATE_GAMEOBJECT,
        ECS_DESTROY_GAMEOBJECT
    };

    struct Event {
        EventType event_type;
        char data[512];
    };

    struct EventData_PROJECT_LOAD {
        Project *project;
    };
    struct EventData_PROJECT_CREATE {
        Project *project;
    };
    struct EventData_PROJECT_LOAD_LATE{
        Project *project;
    };
    struct EventData_WINDOW_RESIZE {
        int width, height;
    };
    struct EventData_WINDOW_CLOSE {
    };
    struct EventData_WINDOW_KEYDOWN {
        KeyCode keycode;
    };
    struct EventData_WINDOW_KEYUP {
        KeyCode keycode;
    };
    struct EventData_WINDOW_KEYPRESSED {
        KeyCode keycode;
    };
    struct EventData_ECS_INSTANTIATE_GAMEOBJECT {
        GameObject game_object;
    };
    struct EventData_ECS_DESTROY_GAMEOBJECT {
        GameObject game_object;
    };
}