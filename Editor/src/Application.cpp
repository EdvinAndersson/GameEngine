#include "Application.h"

namespace CWEditor {
    
    void OnWindowEvent(WindowEventType e, void *data);

    Application::~Application() {
        delete window;
        delete cogwheel;
        delete application_view;
    }

    void Application::Init() {
        cogwheel = new CW::Cogwheel();
        cogwheel->Init();

        window = new Window();
        int success = window->Init(L"Cogwheel Engine", 1920/2, 1080/2);
        if (success == 1) {
            cogwheel->Stop();
            return;
        }
        window->EnableVSync(true);
        window->SetImGUIWindowsProcHandler(ImGui_ImplWin32_WndProcHandler);
        window->on_window_event = OnWindowEvent;

        R3D_Init(window);

        EventListen(CW::WINDOW_CLOSE);
        EventListen(CW::WINDOW_RESIZE);
        EventListen(CW::WINDOW_KEYDOWN);
        EventListen(CW::WINDOW_KEYPRESSED);
        EventListen(CW::PROJECT_LOAD);

        application_view = new ApplicationView();
        application_view->Init(cogwheel, window);

        for (int i = 0; i < 3; i++) {
            CW::GameObject obj = CW::GameObject::Instantiate();
            CW::Transform& transform = obj.GetComponent<CW::Transform>();
            transform.position = Vec3 {-1.5f + i*1.5f, 0, -4.0f };
        }

        cogwheel->GetSceneManager()->CreateNewScene("Test scene");
    }

    void Application::Run() {
        while (cogwheel->IsRunning()) {
            cogwheel->Update();
            
            application_view->Update();

            window->WinSwapBuffers();
            window->PollEvents();
        }
    }
    void Application::OnEvent(CW::Event event) {
        switch (event.event_type)
        {
            case CW::WINDOW_CLOSE:
            {
                cogwheel->Stop();
            } break;
            case CW::WINDOW_RESIZE:
            {
                CW::EventData_WINDOW_RESIZE* e = (CW::EventData_WINDOW_RESIZE*) event.data;
                //R3D__ResizeCallback(e->width, e->height);
            } break;
            case CW::WINDOW_KEYDOWN:
            {
                CW::EventData_WINDOW_KEYDOWN* e = (CW::EventData_WINDOW_KEYDOWN*) event.data;
                if (e->keycode == KeyCode::ESCAPE) {
                    cogwheel->Stop();
                    window->Destroy();
                }
            } break;
            case CW::WINDOW_KEYPRESSED:
            {
                CW::EventData_WINDOW_KEYPRESSED* e = (CW::EventData_WINDOW_KEYPRESSED*) event.data;
                if (e->keycode == KeyCode::F11) {
                    window->WinSetFullscreen(!window->IsFullscreen());
                }
            } break;
            case CW::PROJECT_LOAD:
            {
                CW::EventData_PROJECT_LOAD* e = (CW::EventData_PROJECT_LOAD*) event.data;
                window->WinSetTitle(e->project.specification.project_name);
            } break;
            default:
            {
                Assert(false);
            }
        }
    }

    void OnWindowEvent(WindowEventType e, void *data) {
        switch (e) {
            case WindowEventType::WINDOW_CLOSE: {
                CW::EventManager::InvokeEvent_(CW::WINDOW_CLOSE, data, sizeof(CW::EventData_WINDOW_CLOSE));
            } break;
            case WindowEventType::WINDOW_RESIZE: {
                CW::EventManager::InvokeEvent_(CW::WINDOW_RESIZE, data, sizeof(CW::EventData_WINDOW_RESIZE));
            } break;
            case WindowEventType::WINDOW_KEYDOWN: {
                CW::EventManager::InvokeEvent_(CW::WINDOW_KEYDOWN, data, sizeof(CW::EventData_WINDOW_KEYDOWN));
            } break;
            case WindowEventType::WINDOW_KEYPRESSED: {
                CW::EventManager::InvokeEvent_(CW::WINDOW_KEYPRESSED, data, sizeof(CW::EventData_WINDOW_KEYPRESSED));
            } break;
            case WindowEventType::WINDOW_KEYUP: {
                CW::EventManager::InvokeEvent_(CW::WINDOW_KEYUP, data, sizeof(CW::EventData_WINDOW_KEYUP));
            } break;
            default: {
                printf("Unhandled Window Event...\n");
            }
        }
    }
}