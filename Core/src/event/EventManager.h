#pragma once

#include "IEventListener.h"

namespace CW {

    #define InvokeEvent(event_type, data) InvokeEvent_(event_type, data, sizeof(EventData_##event_type))

    class EventManager {
        public:        
            static void AddEventListener(IEventListener *listener);
            static void InvokeEvent_(EventType event_type, void *data, size_t event_type_size);
    }; 
}