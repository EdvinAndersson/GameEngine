#include "IEventListener.h"
#include "EventManager.h"

namespace CW {
    IEventListener::IEventListener() : bitmask(0) {
        EventManager::AddEventListener(this);
    }

    void IEventListener::OnEvent(Event event) {}

    void IEventListener::EventListen(EventType event_type) {
        bitmask = bitmask | (1 << event_type);
    }
}