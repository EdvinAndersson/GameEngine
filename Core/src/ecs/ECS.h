#pragma once

#include "../event/IEventListener.h"

namespace CW {
    class ECS : public IEventListener {
        public:
            ECS();

            void UpdateComponenets();
            void OnEvent(Event event) override;
    };
}