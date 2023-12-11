#pragma once

#include "Events.h"
#include <cstdint>

namespace CW {

    class IEventListener {
        public:
            IEventListener();
    
            virtual void OnEvent(Event event);
            void EventListen(EventType event_type);

            uint64_t bitmask;
    };
}