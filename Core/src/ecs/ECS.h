#pragma once

#include "../event/IEventListener.h"
#include "../scene/Scene.h"

#include "assets/AssetManager.h"
#include "GameObject.h"
#include "Rendering/Renderer3D.h"

namespace CW {
    class ECS : public IEventListener {
        public:
            ECS(bool build_scripts_dll);

            void UpdateBaseComponents();
            void UpdateComponenets();
            void OnEvent(Event event) override;
        private:
            bool build_scripts_dll;
    };
}