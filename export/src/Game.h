#pragma once

#include "Cogwheel.h"
#include "Window.h"
#include "CWAssert.h"
#include "Rendering/Renderer3D.h"
#include "event/EventManager.h"
#include "Rendering/Framebuffer.h"

namespace CWGame {

    class Game : CW::IEventListener {
        public:
            ~Game();

            void Init();
            void Run();

            void OnEvent(CW::Event event) override;
        private:
            CW::Cogwheel *cogwheel;
            CW::Window *window;

            CW::Framebuffer *framebuffer;
    };
}