#include "Cogwheel.h"
#include "event/EventManager.h"
#include "rendering/Renderer3D.h"
#include "ecs/GameObject.h"

namespace CW {

    #ifdef DEBUG
    #define Assert(expr) if (!expr) { *(int *)0 = 0; }
    #else
    #define Assert(expr)
    #endif

    Cogwheel::Cogwheel() {}

    Cogwheel::~Cogwheel() {
        delete window;
        delete ecs;
        delete scene_manager;
        delete project_manager;
    }

    int Cogwheel::Init() {
        window = new CW::Window();
        int success = window->Init(L"Cogwheel Engine", 1920, 1080);
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
        EventListen(PROJECT_LOAD);

        R3D_Init(window);
        ecs = new ECS();

        project_manager = new ProjectManager();

        ProjectSpecification spec;
        spec.project_name = new char[1024] { "Unnamed Project" };
        spec.vsync = true;
        spec.resolution_mode = ResolutionMode::WINDOWED;
        spec.windowed_size = Vec2 { 1920, 1080 };

        scene_manager = new SceneManager();
        scene_manager->CreateNewScene("Basic Scene");
        
        project_manager->SetSceneManager(scene_manager);

        project_manager->CreateProject(spec);

        return 0;
    }

    void Cogwheel::Update() {
        //Update systems here

        for (GameObject game_object : scene_manager->GetActiveScene().game_objects) {
            Transform& transform = game_object.GetComponent<CW::Transform>();

            R3D_RenderCube(transform.position, transform.scale, Vec3{0,1,0});
        }
    }

    void Cogwheel::Stop() {
        running = false;
        window->Destroy();
    }
    void Cogwheel::SwapBuffersAndPollEvents() {
        window->WinSwapBuffers();
        window->PollEvents();
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
            case PROJECT_LOAD:
            {
                EventData_PROJECT_LOAD* e = (EventData_PROJECT_LOAD*) event.data;
                window->WinSetTitle(e->project.specification.project_name);
                /*window->EnableVSync(e->project.specification.vsync);
                window->WinSetSize(e->project.specification.windowed_size.width, e->project.specification.windowed_size.height);

                if (e->project.specification.resolution_mode == ResolutionMode::FULLSCREEN)
                    window->WinSetFullscreen(true);
                else
                    window->WinSetFullscreen(false);
                */
            } break;
            default:
            {
                Assert(false);
            }
        }
    }
}