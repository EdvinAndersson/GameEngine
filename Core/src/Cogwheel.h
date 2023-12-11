#include "Window.h"
#include "event/IEventListener.h"

namespace CW {

    class Cogwheel : public IEventListener {

        public:
            Cogwheel();

            int Init(const wchar_t *title, int width, int height);
            void Update();
            void Stop();
            inline bool IsRunning() { return running; }
            inline Window* GetWindow() { return window; }

            void OnEvent(Event event) override;
        private:
            CW::Window *window;
            bool running;
    };
}