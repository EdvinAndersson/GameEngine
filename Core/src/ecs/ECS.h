#pragma once

#include "../event/IEventListener.h"
#include "../scene/Scene.h"

#include "assets/AssetManager.h"

namespace CW {
    class ECS : public IEventListener {
        public:
            ECS();

            void UpdateComponenets(Scene &scene);
            void OnEvent(Event event) override;
    };
}