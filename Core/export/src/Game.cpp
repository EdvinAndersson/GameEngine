#include "Game.h"

namespace CWGame {

    Game::~Game() {
        delete window;
        delete cogwheel;
    }

    void Game::Init() {
        window = new CW::Window();

        int success = window->Init(L"Game", 1024, 600);
        CW_ASSERT(success == 0, "Could not create window...");

        cogwheel = new CW::Cogwheel();
        cogwheel->Init();

        CW::R3D_Init(window);

        cogwheel->GetProjectManager()->LoadProject("Unnamed Project.proj");
    }

    void Game::Run() {
        while (cogwheel->IsRunning()) {
            cogwheel->Update();

            CW::R3D_Clear(vec4s {0,0,0,1} );
            CW::Scene& active_scene = cogwheel->GetSceneManager()->GetActiveScene();


            for (CW::GameObject game_object : active_scene.game_objects) {
                //CW::Transform& transform = game_object.GetComponent<CW::Transform>();
            }
            for (CW::GameObject game_object : active_scene.game_objects) {
                if (game_object.HasComponent<CW::MeshRenderer>()) {
                    CW::Transform& transform = game_object.GetComponent<CW::Transform>();
                    CW::MeshRenderer& mesh_renderer = game_object.GetComponent<CW::MeshRenderer>();

                    versors quat = glms_vec3(transform.rotation);
                    CW::R3D_RenderMesh(mesh_renderer.mesh, mesh_renderer.material, transform.position, transform.scale, quat);
                }
            }

            window->PollEvents();
            window->WinSwapBuffers();
        }
    }
}
