#include "Cogwheel.h"
#include "CWAssert.h"
#include "event/EventManager.h"
#include "ecs/GameObject.h"

namespace CW {

    Cogwheel::Cogwheel() {}

    Cogwheel::~Cogwheel() {
        delete asset_manager;
        delete ecs;
        delete scene_manager;
        delete project_manager;
    }

    int Cogwheel::Init(bool build_scripts_dll) {
        running = true;

        EventListen(PROJECT_LOAD);

        asset_manager = new AssetManager();
        ecs = new ECS(build_scripts_dll);
        project_manager = new ProjectManager();

        scene_manager = new SceneManager();
        project_manager->SetSceneManager(scene_manager);

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
                CW_ASSERT(false, "");
            }
        }
    }
}