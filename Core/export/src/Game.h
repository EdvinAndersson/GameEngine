#pragma once

#include "Cogwheel.h"
#include "Window.h"
#include "CWAssert.h"
#include "Rendering/Renderer3D.h"

namespace CWGame {

    class Game {
        public:
            ~Game();

            void Init();
            void Run();
        private:
            CW::Cogwheel *cogwheel;
            CW::Window *window;
    };
}