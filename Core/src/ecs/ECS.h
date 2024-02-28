#pragma once

#include "../event/IEventListener.h"
#include "../scene/Scene.h"

#include "assets/AssetManager.h"
#include "GameObject.h"
#include "Rendering/Renderer3D.h"

#include "Editor/src/vendor/framed/framed.h"

namespace CW {
    class ECS : public IEventListener {
        public:
            ECS();

            void UpdateBaseComponents();
            void UpdateComponenets(Scene &scene);
            void OnEvent(Event event) override;
    };
}