#pragma once
#include "../Window.h"

namespace CW {

    enum EventType {
        WINDOW_CLOSE,
        WINDOW_RESIZE,
        WINDOW_KEYDOWN,
        WINDOW_KEYUP,
        WINDOW_KEYPRESSED
    };

    struct Event {
        EventType event_type;
        char data[512];
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
}