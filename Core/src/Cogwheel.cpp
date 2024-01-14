#include "Cogwheel.h"
#include "event/EventManager.h"
#include "ecs/GameObject.h"

namespace CW {

    #ifdef DEBUG
    #define Assert(expr) if (!expr) { *(int *)0 = 0; }
    #else
    #define Assert(expr)
    #endif

    Cogwheel::Cogwheel() {}

    Cogwheel::~Cogwheel() {
        delete ecs;
        delete scene_manager;
        delete project_manager;
    }

    int Cogwheel::Init() {
        running = true;

        EventListen(PROJECT_LOAD);

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

        
    }

    void Cogwheel::Stop() {
        running = false;
    }
    void Cogwheel::OnEvent(Event event) {

        switch (event.event_type)
        {
            case PROJECT_LOAD:
            {
                EventData_PROJECT_LOAD* e = (EventData_PROJECT_LOAD*) event.data;
            } break;
            default:
            {
                Assert(false);
            }
        }
    }
}