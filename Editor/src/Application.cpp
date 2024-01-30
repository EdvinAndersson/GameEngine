#include "Application.h"
#include "CWAssert.h"

namespace CWEditor {
    
    void OnWindowEvent(CW::WindowEventType e, void *data);

    Application::~Application() {
        delete window;
        delete cogwheel;
        delete application_view;
    }

    void Application::Init() {
        window = new CW::Window();
        int success = window->Init(L"Cogwheel Engine", 1920/2, 1080/2);
        if (success == 1) {
            cogwheel->Stop();
            return;
        }
        window->EnableVSync(true);
        window->SetImGUIWindowsProcHandler(ImGui_ImplWin32_WndProcHandler);
        window->on_window_event = OnWindowEvent;

        cogwheel = new CW::Cogwheel();
        cogwheel->Init();

        CW::R3D_Init(window);
        
        #if 1
        { //Create project
            CW::ProjectSpecification spec;
            spec.project_name = "Unnamed Project";
            spec.vsync = true;
            spec.resolution_mode = CW::ResolutionMode::WINDOWED;
            spec.windowed_size = Vec2 { 1920, 1080 };

            CW::Project *project = cogwheel->GetProjectManager()->CreateProject("Editor/res/projects/Project1", spec);
            cogwheel->GetProjectManager()->LoadProject(project);
            cogwheel->GetSceneManager()->CreateNewScene("Basic Scene");
        }
        #endif

        //cogwheel->GetProjectManager()->LoadProject("Editor/res/projects/Project1/Unnamed Project.proj");
        
        EventListen(CW::WINDOW_CLOSE);
        EventListen(CW::WINDOW_RESIZE);
        EventListen(CW::WINDOW_KEYDOWN);
        EventListen(CW::WINDOW_KEYPRESSED);
        EventListen(CW::PROJECT_LOAD);

        application_view = new ApplicationView();
        application_view->Init(cogwheel, window);
    }

    void Application::Run() {
        while (cogwheel->IsRunning()) {
            cogwheel->Update();
            
            application_view->Update();

            window->PollEvents();
            window->WinSwapBuffers();
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
            } break;
            case CW::WINDOW_KEYDOWN:
            {
                CW::EventData_WINDOW_KEYDOWN* e = (CW::EventData_WINDOW_KEYDOWN*) event.data;
                if (e->keycode == CW::KeyCode::ESCAPE) {
                    cogwheel->Stop();
                    window->Destroy();
                }
            } break;
            case CW::WINDOW_KEYPRESSED:
            {
                CW::EventData_WINDOW_KEYPRESSED* e = (CW::EventData_WINDOW_KEYPRESSED*) event.data;
                if (e->keycode == CW::KeyCode::F11) {
                    window->WinSetFullscreen(!window->IsFullscreen());
                }
            } break;
            case CW::PROJECT_LOAD:
            {
                CW::EventData_PROJECT_LOAD* e = (CW::EventData_PROJECT_LOAD*) event.data;
                window->WinSetTitle(e->project->specification.project_name);
            } break;
            default:
            {
                CW_ASSERT(false, "");
            }
        }
    }

    void OnWindowEvent(CW::WindowEventType e, void *data) {
        switch (e) {
            case CW::WindowEventType::EVENT_WINDOW_CLOSE: {
                CW::EventManager::InvokeEvent_(CW::WINDOW_CLOSE, data, sizeof(CW::EventData_WINDOW_CLOSE));
            } break;
            case CW::WindowEventType::EVENT_WINDOW_RESIZE: {
                CW::EventManager::InvokeEvent_(CW::WINDOW_RESIZE, data, sizeof(CW::EventData_WINDOW_RESIZE));
            } break;
            case CW::WindowEventType::EVENT_WINDOW_KEYDOWN: {
                CW::EventManager::InvokeEvent_(CW::WINDOW_KEYDOWN, data, sizeof(CW::EventData_WINDOW_KEYDOWN));
            } break;
            case CW::WindowEventType::EVENT_WINDOW_KEYPRESSED: {
                CW::EventManager::InvokeEvent_(CW::WINDOW_KEYPRESSED, data, sizeof(CW::EventData_WINDOW_KEYPRESSED));
            } break;
            case CW::WindowEventType::EVENT_WINDOW_KEYUP: {
                CW::EventManager::InvokeEvent_(CW::WINDOW_KEYUP, data, sizeof(CW::EventData_WINDOW_KEYUP));
            } break;
            default: {
                printf("Unhandled Window Event...\n");
            }
        }
    }
}