#include "EventManager.h"
#include <memory>

namespace CW {

    #define MAX_EVENT_LISTERNERS 100

    static IEventListener *event_listeners[MAX_EVENT_LISTERNERS];
    static unsigned int event_listener_count = 0;

    void EventManager::AddEventListener(IEventListener *listener) {
        event_listeners[event_listener_count] = listener;

        event_listener_count++;
    }
    void EventManager::InvokeEvent_(EventType event_type, void *data, size_t event_type_size) {
        Event event;
        event.event_type = event_type;

        if (data != 0)
            memcpy(event.data, data, event_type_size);

        uint64_t bitmask = 1 << event_type;

        for (int i = 0; i < event_listener_count; i++) {
            if ((event_listeners[i]->bitmask & bitmask) != 0)
                event_listeners[i]->OnEvent(event);
        }
    }
}