#include "ApplicationView.h"

namespace CWEditor {

    ApplicationView::ApplicationView() {}

    void ApplicationView::Init(CW::Cogwheel *_cogwheel, CW::Window *_window) {
        cogwheel = _cogwheel;
        window = _window;

        framebuffer_game_view = new CW::Framebuffer(window->GetWidth(), window->GetHeight());

        EventListen(CW::EventType::WINDOW_CLOSE);
        EventListen(CW::EventType::WINDOW_RESIZE);

        glDisable(GL_FRAMEBUFFER_SRGB);

        //Initialize ImGUI
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;    // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;       // Enable Docking
        ImGui::StyleColorsDark();
        ImGui_ImplWin32_InitForOpenGL(window->GetHandle());
        ImGui_ImplOpenGL3_Init();
    }

    void ApplicationView::Update() {
        CW::R3D_Clear(Vec4 {0,0,0,1} );

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplWin32_NewFrame();

        ImGui::NewFrame();

        RenderDockspace();
        
        {
            ImGui::Begin("Game View");

            framebuffer_game_view->Bind();

            CW::R3D_Clear(Vec4 {0,0,0,1} );
            
            CW::Scene& active_scene = cogwheel->GetSceneManager()->GetActiveScene();


            for (CW::GameObject game_object : active_scene.game_objects) {
                //CW::Transform& transform = game_object.GetComponent<CW::Transform>();
            }
            for (CW::GameObject game_object : active_scene.game_objects) {
                if (game_object.HasComponent<CW::MeshRenderer>()) {
                    CW::Transform& transform = game_object.GetComponent<CW::Transform>();
                    CW::MeshRenderer& mesh_renderer = game_object.GetComponent<CW::MeshRenderer>();

                    Quaternion quat = QuatEuler(transform.rotation);
                    CW::R3D_RenderMesh(mesh_renderer.mesh, mesh_renderer.material, transform.position, transform.scale, quat);
                }
            }

            framebuffer_game_view->UnBind();

            float width = ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x;
            float height = width * aspect_ratio;
            
            ImGui::Image((ImTextureID)framebuffer_game_view->GetTexture().id, ImVec2 {width, height });
            
            ImGui::End();
        }

        ImGui::Begin("Debug View");

        if (ImGui::Button("Save project")) {
            cogwheel->GetProjectManager()->SaveProject();
        }
        if (ImGui::Button("Load project 1")) {
            cogwheel->GetProjectManager()->LoadProject("Editor/res/projects/Project1/Unnamed Project.proj");
        }
        if (ImGui::Button("Load project 2")) {
            cogwheel->GetProjectManager()->LoadProject("Editor/res/projects/Project2/Unnamed Project.proj");
        }
        static CW::MaterialIndex material;
        if (ImGui::Button("Create Random Cube")) {
            CW::GameObject obj = CW::GameObject::Instantiate(Vec3 {(float) (CW::Random()*2.0f-1)*5, (float) (CW::Random()*2.0f-1)*5, -5-(float) CW::Random()*5.0f });
            CW::MeshRenderer& mesh_renderer = obj.AddComponent<CW::MeshRenderer>();
            mesh_renderer.mesh = CW::AssetManager::Get()->GetDefaultMeshIndex();
            mesh_renderer.material = material;
        }
        if (ImGui::Button("Material 1")) {
            material = CW::AssetManager::Get()->GetMaterialIndex("Material1.mat");
        }
        if (ImGui::Button("Material 2")) {
            material = CW::AssetManager::Get()->GetMaterialIndex("Material2.mat");
        }
        if (ImGui::Button("Create And Load Material 1")) {
            CW::Material mat = {};
            mat.albedo_color = Vec3 { 1.0f, 1.0f, 1.0f };
            mat.albedo = CW::AssetManager::Get()->GetTextureIndex("images/BrickTexture.png");
            CW::AssetManager::Get()->CreateAndLoadMaterialAsset("Material1.mat", mat);
        }
        if (ImGui::Button("Create And Load Material 2")) {
            CW::Material mat = {};
            mat.albedo_color = Vec3 { 0.0f, 0.0f, 1.0f };
            mat.albedo = CW::AssetManager::Get()->GetDefaultTextureIndex();
            CW::AssetManager::Get()->CreateAndLoadMaterialAsset("Material2.mat", mat);
        }
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void ApplicationView::OnEvent(CW::Event e) {
        switch (e.event_type) {
            case CW::EventType::WINDOW_CLOSE: {
                ImGui_ImplOpenGL3_Shutdown();
                ImGui_ImplWin32_Shutdown();
                ImGui::DestroyContext();
            } break;
            case CW::EventType::WINDOW_RESIZE: {
                CW::EventData_WINDOW_RESIZE *data = (CW::EventData_WINDOW_RESIZE*) e.data;
                int width = data->width;
                int height = (int) (data->width * aspect_ratio);

                framebuffer_game_view->ReCreate(width, height);
                CW::R3D_Resize(width, height);
            } break;
        }
    }
    void ApplicationView::RenderDockspace() {
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None; // Config flags for the Dockspace

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

        // If so, get the main viewport:
        const ImGuiViewport* viewport = ImGui::GetMainViewport();

        // Set the parent window's position, size, and viewport to match that of the main viewport. This is so the parent window
        // completely covers the main viewport, giving it a "full-screen" feel.
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);

        // Set the parent window's styles to match that of the main viewport:
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f); // No corner rounding on the window
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f); // No border around the window

        // Manipulate the window flags to make it inaccessible to the user (no titlebar, resize/move, or navigation)
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
        // and handle the pass-thru hole, so the parent window should not have its own background:
        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        ImGui::Begin("DockSpace Demo", (bool *)1, window_flags);

        ImGui::PopStyleVar();

        ImGui::PopStyleVar(2);

        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {   
                if (ImGui::MenuItem("New Project")){
                    
                }

                if (ImGui::MenuItem("Close"))
                    ImGui::CloseCurrentPopup();

                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Edit"))
            {
                if (ImGui::MenuItem("Close"))
                    ImGui::CloseCurrentPopup();

                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        ImGui::End();
    }
}