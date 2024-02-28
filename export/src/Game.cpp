#include "Game.h"

namespace CWGame {

    void OnWindowEvent(CW::WindowEventType e, void *data);

    Game::~Game() {
        delete window;
        delete cogwheel;
    }

    void Game::Init() {
        window = new CW::Window();

        int success = window->Init(L"Game", 1024, 600);
        CW_ASSERT(success == 0, "Could not create window...");

        cogwheel = new CW::Cogwheel();
        cogwheel->Init(false);

        window->on_window_event = OnWindowEvent;

        EventListen(CW::EventType::WINDOW_CLOSE);
        EventListen(CW::EventType::WINDOW_RESIZE);
        EventListen(CW::EventType::PROJECT_LOAD);

        CW::R3D_Init(window);

        cogwheel->GetProjectManager()->LoadProject("Unnamed Project.proj");
    }

    void Game::Run() {
        while (cogwheel->IsRunning()) {
            cogwheel->Update();

            mat4s view = GLMS_MAT4_IDENTITY_INIT;
            view = glms_translate(view, {0,0,0});
            view = glms_mat4_mul(glms_quat_mat4(glms_euler_xyz_quat({0,0,0})), view);
            CW::R3D_SetViewModel(view);
            CW::R3D_SetViewPos({0,0,0});

            glViewport(0, 0, window->GetWidth(), window->GetHeight());
            CW::R3D_Clear(vec4s {0,0,0,1} );
            
            CW::R3D_UseDefaultShader();
            vec3s light_pos = {-12.0f, -12.0f, 12.0f};
            CW::R3D_GetDefaultShader().SetV3("dirLight.direction", vec3s {-light_pos.x,-light_pos.y,-light_pos.z});

            CW::R3D_SetPointLight(vec3s {3.0f, 0, 3.0f}, vec3s {0.4f, 0.4f, 0.4f}, vec3s {1.0f, 1.0f, 1.0f}, vec3s {0.5f, 0.5f, 0.5f}, 0.1, 0.3, 0.4f);

            CW::Scene& active_scene = cogwheel->GetSceneManager()->GetActiveScene();

            cogwheel->GetECS()->UpdateComponenets(active_scene);

            window->PollEvents();
            window->WinSwapBuffers();
        }
    }
    void Game::OnEvent(CW::Event event) {
        switch (event.event_type)
        {
            case CW::WINDOW_CLOSE: {
                cogwheel->Stop();
            } break;
            case CW::PROJECT_LOAD: {
                CW::EventData_PROJECT_LOAD* e = (CW::EventData_PROJECT_LOAD*) event.data;
                window->WinSetTitle(e->project->specification.project_name);
            } break;
            case CW::EventType::WINDOW_RESIZE: {
                CW::EventData_WINDOW_RESIZE *data = (CW::EventData_WINDOW_RESIZE*) event.data;
                //int width = data->width;
                //int height = (int) (data->width * aspect_ratio);

                CW::R3D_Resize(data->width, data->height);
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
