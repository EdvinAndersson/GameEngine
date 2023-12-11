#include "Cogwheel.h"
#include "event/EventManager.h"
#include "rendering/Renderer3D.h"

namespace CW {

    #ifdef DEBUG
    #define Assert(expr) if (!expr) { *(int *)0 = 0; }
    #else
    #define Assert(expr)
    #endif

    Cogwheel::Cogwheel() {}

    int Cogwheel::Init(const wchar_t *title, int width, int height) {
        window = new CW::Window();
        int success = window->Init(title, width, height);
        if (success == 1) {
            Stop();
            return 0;
        }
        window->EnableVSync(true);
        running = true;

        EventListen(WINDOW_CLOSE);
        EventListen(WINDOW_RESIZE);
        EventListen(WINDOW_KEYDOWN);
        EventListen(WINDOW_KEYPRESSED);

        CW::R3D_Init(window);

        return 0;
    }

    void Cogwheel::Update() {
        //Update systems here

        window->WinSwapBuffers();
        window->PollEvents();
    }

    void Cogwheel::Stop() {
        running = false;
        window->Destroy();
    }

    void Cogwheel::OnEvent(Event event) {

        switch (event.event_type)
        {
            case WINDOW_CLOSE:
            {
                running = false;
            } break;
            case WINDOW_RESIZE:
            {
                EventData_WINDOW_RESIZE* e = (EventData_WINDOW_RESIZE*) event.data;
                R3D__ResizeCallback(e->width, e->height);
            } break;
            case WINDOW_KEYDOWN:
            {
                EventData_WINDOW_KEYDOWN* e = (EventData_WINDOW_KEYDOWN*) event.data;
                if (e->keycode == KeyCode::ESCAPE) {
                    Stop();
                }
            } break;
            case WINDOW_KEYPRESSED:
            {
                EventData_WINDOW_KEYPRESSED* e = (EventData_WINDOW_KEYPRESSED*) event.data;
                if (e->keycode == KeyCode::F11) {
                    window->WinSetFullscreen(!window->IsFullscreen());
                }
            } break;
            default:
            {
                Assert(false);
            }
        }
    }
}