#include "ApplicationView.h"

#include "cglm/struct.h"

namespace CWEditor {

    ApplicationView::ApplicationView() {}

    ApplicationView::~ApplicationView() {
        delete assets_builder;
        delete framebuffer_game_view;
        delete framebuffer_dev_view;
        delete console;
    }

    void ApplicationView::Init(CW::Cogwheel *_cogwheel, CW::Window *_window) {
        cogwheel = _cogwheel;
        window = _window;

        console = new Console();

        assets_builder = new AssetsBuilder();
        assets_builder->Refresh();
        current_asset_folder_hash = CW::HashString("");

        framebuffer_game_view = new CW::Framebuffer(CW::FramebufferType::DEFUALT, window->GetWidth(), window->GetHeight());
        framebuffer_dev_view = new CW::Framebuffer(CW::FramebufferType::DEFUALT, window->GetWidth(), window->GetHeight());

        EventListen(CW::EventType::WINDOW_CLOSE);
        EventListen(CW::EventType::WINDOW_RESIZE);
        EventListen(CW::EventType::PROJECT_LOAD);
        EventListen(CW::EventType::PROJECT_LOAD_LATE);

        glDisable(GL_FRAMEBUFFER_SRGB);

        //Initialize ImGUI
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;    // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;       // Enable Docking
        ImGui_ImplWin32_InitForOpenGL(window->GetHandle());
        ImGui_ImplOpenGL3_Init();
        ImGuiStyleGray();

        skybox_texture = CW::CreateCubemapTexture(
            CW::AssetManager::Get()->GetTextureIndex("images/skybox/right.jpg"),
            CW::AssetManager::Get()->GetTextureIndex("images/skybox/left.jpg"),
            CW::AssetManager::Get()->GetTextureIndex("images/skybox/top.jpg"),
            CW::AssetManager::Get()->GetTextureIndex("images/skybox/bottom.jpg"),
            CW::AssetManager::Get()->GetTextureIndex("images/skybox/front.jpg"),
            CW::AssetManager::Get()->GetTextureIndex("images/skybox/back.jpg"));
        
        CW::GameObject camera = CW::GameObject::Instantiate(vec3s {0, 4, 0 });
        CW::Camera& c = camera.AddComponent<CW::Camera>();
        c.is_main = true;
        strcpy(camera.GetComponent<CW::Transform>().name, "Camera");

        CW::GameObject obj = CW::GameObject::Instantiate(vec3s {0, -4, 0 });
        obj.GetComponent<CW::Transform>().scale = vec3s {40, 1, 40};
        CW::MeshRenderer& mesh_renderer = obj.AddComponent<CW::MeshRenderer>();
        mesh_renderer.mesh = CW::AssetManager::Get()->GetDefaultMeshIndex();
        mesh_renderer.materials[0] = CW::AssetManager::Get()->GetDefaultMaterialIndex();
        mesh_renderer.material_count++;
    }
    void ApplicationView::Update() {
        double time = window->GetTime();
        double delta_time = time - previous_time;
        double fps = 1 / delta_time;

        static double second_timer = 0;
        second_timer += delta_time;
        
        if (second_timer >= 1) {
            second_timer = 0;
            printf("FPS: %f\n", fps);
        }

        previous_time = time;

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        RenderDockspace();
        {
            ImGui::Begin("Game View");

            //Shadow pass
            CW::R3D_BeginShadowPass(light_pos);
            RenderScene();
            CW::R3D_EndShadowPass();

            //Render pass
            framebuffer_game_view->Bind();
            glViewport(0, 0, window->GetWidth(), window->GetHeight());
            CW::R3D_Clear(vec4s {0,0,0,1} );

            CW::R3D_UseDefaultShader();
            CW::R3D_GetDefaultShader().SetV3("dirLight.direction", vec3s {-light_pos.x,-light_pos.y,-light_pos.z});

            CW::R3D_SetPointLight(vec3s {3.0f, 0.0f, 3.0f}, vec3s {0.4f, 0.4f, 0.4f}, vec3s {1.0f, 1.0f, 1.0f}, vec3s {0.5f, 0.5f, 0.5f}, 0.1f, 0.3f, 0.4f);
            
            RenderScene();

            CW::R3D_RenderSkybox(skybox_texture->texture, CW::g_main_camera->view);
            
            framebuffer_game_view->UnBind();

            //Show the final render
            float width = ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x;
            float height = width * aspect_ratio;
            ImGui::Image((ImTextureID)framebuffer_game_view->GetTexture().id, ImVec2 {width, height });
            
            ImGui::End();
        }
        mat4s dev_view = GLMS_MAT4_IDENTITY_INIT;
        dev_view = glms_translate(dev_view, dev_pos);
        dev_view = glms_scale(dev_view, vec3s { 1.0f, -1.0f, 1.0f });
        CW::R3D_SetViewModel(dev_view);
        CW::R3D_SetViewPos(dev_pos);
        {
            ImGui::Begin("Dev View");
            static bool down = false;
            if (ImGui::IsWindowHovered()) {
                if (ImGui::IsMouseDown(ImGuiMouseButton_Right)) {
                    if (!down) {
                        window->WinShowCursor(false);
                        down = true;
                    }
                } else {
                    if (down) {
                        window->WinShowCursor(true);
                        down = false;
                    }
                }

                //window->WinShowCursor(!down); TODO: FIX
                if (down) {
                    if (window->GetInputState(CW::W)) {
                        dev_pos.z += 0.1f;
                    }
                    if (window->GetInputState(CW::A)) {
                        dev_pos.x += 0.1f;
                    }
                    if (window->GetInputState(CW::S)) {
                        dev_pos.z -= 0.1f;
                    }
                    if (window->GetInputState(CW::D)) {
                        dev_pos.x -= 0.1f;
                    }
                    if (window->GetInputState(CW::SPACE)) {
                        dev_pos.y += 0.1f;
                    }
                    if (window->GetInputState(CW::SHIFT)) {
                        dev_pos.y -= 0.1f;
                    }
                }
            }

            //Shadow pass
            CW::R3D_BeginShadowPass(light_pos);
            RenderScene();
            CW::R3D_EndShadowPass();

            //Render pass
            framebuffer_dev_view->Bind();
            glViewport(0, 0, window->GetWidth(), window->GetHeight());
            CW::R3D_Clear(vec4s {0,0,0,1} );

            CW::R3D_UseDefaultShader();
            CW::R3D_GetDefaultShader().SetV3("dirLight.direction", vec3s {-light_pos.x,-light_pos.y,-light_pos.z});

            static float rot = 0;
            rot += 0.004f;
            vec3s p = vec3s{cosf(rot)*3, 0, sinf(rot)*3};
            CW::R3D_SetPointLight(p, vec3s {0.4f, 0.4f, 0.4f}, vec3s {1.0f, 1.0f, 1.0f}, vec3s {0.5f, 0.5f, 0.5f}, 0.1, 0.3, 0.4f);
            CW::MaterialIndex m[8] = {CW::AssetManager::Get()->GetDefaultMaterialIndex()};
            CW::R3D_RenderMesh(CW::AssetManager::Get()->GetDefaultMeshIndex(), m, 1, p, vec3s {0.2f, 0.2f, 0.2f}, GLMS_QUAT_IDENTITY_INIT);
            
            RenderScene();

            CW::R3D_RenderSkybox(skybox_texture->texture, dev_view);
            
            framebuffer_dev_view->UnBind();

            //Show the final render
            float width = ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x;
            float height = width * aspect_ratio;
            ImGui::Image((ImTextureID)framebuffer_dev_view->GetTexture().id, ImVec2 {width, height });
            
            ImGui::End();
        }
        {
            //ImGui::ShowDemoWindow();
            ImGui::Begin("Scene objects");
            CW::Scene& active_scene = cogwheel->GetSceneManager()->GetActiveScene();
            static int selection_mask = (1 << 2);
            int node_clicked = -1;

            for (int i = 0; i < active_scene.game_objects.size(); i++){
                CW::GameObject game_object = *std::next(active_scene.game_objects.begin(), i); 
                char* game_object_name = game_object.GetComponent<CW::Transform>().name;
                ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;

                const bool is_selected = (selection_mask & (1 << i)) != 0;
                if (is_selected)
                    node_flags |= ImGuiTreeNodeFlags_Selected;
            
                bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, game_object_name);
                if (ImGui::IsItemClicked()) {
                    selected_game_object = game_object;
                    selected_asset = {};
                    node_clicked = i;
                }
                
                if (ShowPopup(game_object)) {
                    node_clicked = i;
                }
                if (node_clicked != -1) {
                    
                    if (ImGui::GetIO().KeyCtrl)
                        selection_mask ^= (1 << node_clicked);         
                    else 
                        selection_mask = (1 << node_clicked);           
            
                }
                if (node_open) {
                    ImGui::TreePop();  
                }
            }
            
            ShowPopup(CW::GameObject {});

            if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImGui::IsWindowHovered() && node_clicked == -1){
                ImGui::OpenPopup("Popup");
            }
                
            ImGui::End();
        }
        {
            //TODO
            //ImGuiCond_FirstUseEver
            //ImGui::SetNextWindowPos();
            //ImGui::SetNextWindowSize();

            ImGui::Begin("Components");
            if (selected_game_object.entity != 0)
                RenderComponents();
            if (selected_asset.asset_index != 0)
                RenderAssetInspector();

            ImGui::End();
        }
        {
            ImGui::Begin("Assets");
            bool asset_update = RenderAssetCreationPopup();
            if (asset_update) assets_builder->Refresh();

            if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows)) {
                OpenAssetCreationPopup();
                selected_asset_info = 0;
            }
            RenderAssets();
            ImGui::End();
        }
        {
            ImGui::Begin("Console");
            console->Render();
            ImGui::End();
        }
        {
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }
    }

    void ApplicationView::RenderScene() {
        cogwheel->GetECS()->UpdateComponenets();
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
            case CW::EventType::PROJECT_LOAD: {
                selected_game_object = CW::GameObject {};
                selected_asset = {};
            } break;
            case CW::EventType::PROJECT_LOAD_LATE: {
                assets_builder->Refresh();
                current_asset_folder_hash = CW::HashString("");
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

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        ImGui::Begin("DockSpace", (bool *)1, window_flags);

        ImGui::PopStyleVar();
        ImGui::PopStyleVar(2);

        { //Header
            ImGuiStyle& style = ImGui::GetStyle();
            float width = 0.0f;
            width += ImGui::CalcTextSize("Play").x;
            width += ImGui::CalcTextSize("Pause!").x;
            width += style.ItemSpacing.x;

            float off = (ImGui::GetContentRegionAvail().x - width) * 0.5f;
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);

            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3);
            if (ImGui::Button("Play")) {
            }
            ImGui::SameLine();
            if (ImGui::Button("Pause")) {
            }
            ImGui::PopStyleVar();
        }

        // Project Settings
        if(show_project_settings)
            RenderProjectSettings();

        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        
        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("File")) {   
                if (ImGui::MenuItem("New Project")) {
                    
                }
                if (ImGui::MenuItem("Load Project")) {
                    cogwheel->GetProjectManager()->LoadProject((char*)GetProjectFilePath().c_str());
                }
                if(ImGui::MenuItem("Save project")){
                    cogwheel->GetProjectManager()->SaveProject();
                }
                if(ImGui::MenuItem("Export project")){
                    cogwheel->GetProjectManager()->SaveProject();
                    STARTUPINFOA si = {};
                    PROCESS_INFORMATION pi = {};
                    bool success = CreateProcessA(NULL, "export\\export.bat", NULL, NULL, false, 0, NULL, NULL, &si, &pi);
                    
                    CW_ASSERT(success != 0, "Could not build the game!");
                }
                if (ImGui::MenuItem("Project Settings")) {
                    ShowProjectSettings(true);
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
    void ApplicationView::RenderAssetInspector() {
        CW::Texture texture = CW::AssetManager::Get()->GetTextureData(selected_asset.icon)->texture;
        ImGui::Image((ImTextureID) texture.id, ImVec2 { 80, 80 });
        ImGui::SameLine();
        ImGui::Text("%s", selected_asset.name);
        ImGui::Separator();

        switch (selected_asset.asset_type)
        {
            case AssetType::TEXTURE: {
                CW::TextureData *texture_data = CW::AssetManager::Get()->GetTextureData(selected_asset.asset_index);

                ImGui::Text("Width: %i", texture_data->width);
                ImGui::Text("Height: %i", texture_data->height);
            } break;
            case AssetType::MATERIAL: {
                CW::Material *material = CW::AssetManager::Get()->GetMaterial(selected_asset.asset_index);
                bool updated = false;

                if (UIDragFloat3("Albedo Color", "Albedo Color", &material->albedo_color)) updated = true;
                if (UIAssetInput(AssetType::TEXTURE, "Albedo", &material->albedo)) updated = true;
                if (UIAssetInput(AssetType::TEXTURE, "Normal", &material->normal_map)) updated = true;
                if (UIAssetInput(AssetType::TEXTURE, "Specular", &material->specular_map)) updated = true;

                if (updated) {
                    Console::Log("Updated!");
                    CW::AssetManager::Get()->CreateAndLoadMaterialAsset(material->asset_path, *material);
                }
            } break;
            case AssetType::MESH: {

            } break;
            case AssetType::MODEL: {

            } break;
            case AssetType::SCRIPT: {

            } break;
        }
    }
    void ApplicationView::RenderAssets() {
        static char* asset_path = "";

        if (ImGui::Button("Refresh")) {
            assets_builder->Refresh();
        }
        ImGui::SameLine();
        if (ImGui::Button("Back")) {
            asset_path = GetSubDirectory(asset_path);
            current_asset_folder_hash = CW::HashString(asset_path);
        }

        ImGuiStyle& style = ImGui::GetStyle();
        float window_visible = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;

        AssetInfoArray& asset_info_array = (*assets_builder->GetContents())[current_asset_folder_hash];
        int asset_info_count = assets_builder->GetContentsCount(current_asset_folder_hash);

        for (int i = 0; i < asset_info_count; i++) { 
            ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollbar;

            ImGui::PushID(i);
            ImGui::BeginChild("Asset", ImVec2(asset_view_size.x, asset_view_size.y), 0, window_flags);

            AssetInfo& asset_info = *asset_info_array.asset_infos[i];
            CW::Texture texture = CW::AssetManager::Get()->GetTextureData(asset_info.icon)->texture;

            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.f, 0.f, 0.f, 0.f));

            bool is_clicked = ImGui::ImageButton((ImTextureID) texture.id, ImVec2 { asset_view_size.x - style.ItemSpacing.x, asset_view_size.x - style.ItemSpacing.x }, ImVec2(0,0), ImVec2(1,1), -1, ImVec4(0,0,0,0), ImVec4(1,1,1,1));
            if (is_clicked) {
                if (asset_info.asset_type == AssetType::FOLDER) {
                    asset_path = asset_info.path;
                    current_asset_folder_hash = CW::HashString(asset_path);
                } else {
                    selected_asset = asset_info;
                    selected_game_object = CW::GameObject {};
                    is_clicked = false;
                }
            }
            if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
                selected_asset_info = &asset_info;
            }

            if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
                ImGui::SetDragDropPayload(GetDragDropType(asset_info.asset_type), &asset_info.asset_index, sizeof(size_t));
                ImGui::Text("Drag");
                ImGui::EndDragDropSource();
            }
            ImGui::TextWrapped("%s", asset_info.name);

            ImGui::PopStyleColor(1);
            ImGui::EndChild();
            ImGui::PopID();

            float next_button = ImGui::GetItemRectMax().x + style.ItemSpacing.x + asset_view_size.x;
            if (next_button < window_visible)
                ImGui::SameLine();

            if (is_clicked) {
                selected_asset_info = 0;
                return;
            }
        }
    }
    void ApplicationView::RenderComponents(){
        ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanAvailWidth;
        
        int index = 0;

        bool node_open = ImGui::TreeNodeEx((void*)(intptr_t) index++, node_flags, "Transform");
        if(node_open){
            CW::Transform& transform = selected_game_object.GetComponent<CW::Transform>();

            UIDragFloat3("Position", "Position (X, Y, Z)", &transform.position);
            UIDragFloat3("Rotation", "Rotation (X, Y, Z)", &transform.rotation);
            UIDragFloat3("Scale", "Scale (X, Y, Z)", &transform.scale);

            ImGui::TreePop();
        }
        if (selected_game_object.HasComponent<CW::Camera>()) {
            node_open = ImGui::TreeNodeEx((void*)(intptr_t) index++, node_flags, "Camera");
            if (ImGui::BeginPopupContextItem("PopupComponent")) {
                if (ImGui::Button("Remove")) {
                    selected_game_object.RemoveComponent<CW::Camera>();
                    ImGui::EndPopup();
                    if(node_open) ImGui::TreePop();
                    return;
                }
                ImGui::EndPopup();
            }
            if (node_open) {
                CW::Camera& camera = selected_game_object.GetComponent<CW::Camera>();
                UICheckbox("Main Camera", &camera.is_main);
                ImGui::TreePop();
            }
        }
        if (selected_game_object.HasComponent<CW::MeshRenderer>()) {
            node_open = ImGui::TreeNodeEx((void*)(intptr_t) index++, node_flags, "MeshRenderer");
            if (ImGui::BeginPopupContextItem("PopupComponent")) {
                if (ImGui::Button("Remove")) {
                    selected_game_object.RemoveComponent<CW::MeshRenderer>();
                    ImGui::EndPopup();
                    if(node_open) ImGui::TreePop();
                    return;
                }
                ImGui::EndPopup();
            }
            if (node_open) {
                CW::MeshRenderer& mesh_renderer = selected_game_object.GetComponent<CW::MeshRenderer>();
                UIAssetInput(AssetType::MESH, "Mesh", &mesh_renderer.mesh);
                UIAssetInputList(AssetType::MATERIAL, mesh_renderer.materials, &mesh_renderer.material_count);
                ImGui::TreePop();
            }
        }
        if(selected_game_object.HasComponent<CW::Light>()){
            node_open = ImGui::TreeNodeEx((void*)(intptr_t) index++, node_flags, "Light");
            if (ImGui::BeginPopupContextItem("PopupComponent")) {
                if (ImGui::Button("Remove")) {
                    selected_game_object.RemoveComponent<CW::Light>();
                    ImGui::EndPopup();
                    if(node_open) ImGui::TreePop();
                    return;
                }
                ImGui::EndPopup();
            }
            if(node_open){
                ImGui::TreePop();
            }
        }
        
        for (auto& it : *CW::AssetManager::Get()->GetLoadedScripts()) {
            CW::ScriptData *script_data = it.second;
            if (CW::HasGeneratedComponent(CW::HashString(script_data->name), selected_game_object)) {
                node_open = ImGui::TreeNodeEx((void*)(intptr_t) index++, node_flags, script_data->name);
                if (ImGui::BeginPopupContextItem("PopupComponent")) {
                    if (ImGui::Button("Remove")) {
                        CW::RemoveGeneratedComponent(CW::HashString(script_data->name), selected_game_object);
                        ImGui::EndPopup();
                        if(node_open) ImGui::TreePop();
                        return;
                    }
                    ImGui::EndPopup();
                }
                if(node_open) {
                    ImGui::TreePop();
                }
            }
        }
        RenderAssetPopup(selected_game_object, AssetType::SCRIPT);
        if(ImGui::Button("Add Component")){
            OpenAssetPopup();
        }
    }

    bool ApplicationView::ShowPopup(CW::GameObject game_object){
        bool is_clicked = false;
        char title[128] = "Popup";
        bool flags = ImGuiPopupFlags_None;

        if(game_object.entity != 0){
            strcpy(title, game_object.GetComponent<CW::Transform>().name);
            flags = ImGuiPopupFlags_MouseButtonRight;
        } else {
            ImGui::Text("");
        }

        if (ImGui::BeginPopupContextItem(title, flags)) { 
            bool enter_pressed = false;
            is_clicked = true;
            if (game_object.entity != 0) {
                if (ImGui::BeginPopupContextItem("New_Name_Popup")){
                    static char buf1[32] = ""; 
                    ImGui::InputText("default", buf1, 32);

                    if(window->GetInputState(CW::KeyCode::RETURN)){
                        enter_pressed = true;

                        if(CheckNameConflict(buf1) == false)
                            strcpy_s(game_object.GetComponent<CW::Transform>().name, 32, buf1);

                        strcpy_s(buf1, 1, "");
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::EndPopup(); 
                }

                if (ImGui::Button("New Name"))
                    ImGui::OpenPopup("New_Name_Popup");

                if(ImGui::Button("Duplicate")){
                    CW::Transform& transform = selected_game_object.GetComponent<CW::Transform>();
                    
                    CW::GameObject new_obj = CW::GameObject::Instantiate();
                    CW::Transform& new_transform = new_obj.GetComponent<CW::Transform>();
                    new_transform.position = transform.position;
                    new_transform.rotation = transform.rotation;
                    new_transform.scale = transform.scale;

                    //selected_game_object = new_obj;

                    ImGui::CloseCurrentPopup();
                }
                if(ImGui::Button("Remove")){
                    CW::GameObject::Destory(game_object);
                    selected_game_object = CW::GameObject {0};
                    Console::Log(LogLevel::LOG_ERROR, "Removed GameObject");
                    ImGui::CloseCurrentPopup();
                }
            }
            if(ImGui::Button("New Game Object")) {
                CW::GameObject::Instantiate();
                Console::Log("Added a new GameObject");
                ImGui::CloseCurrentPopup();
            }
            if (ImGui::Button("Close") || enter_pressed) {
                Console::Log(LogLevel::LOG_WARNING, "Closed popup");
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
        return is_clicked;
    }
    bool ApplicationView::RenderAssetCreationPopup() {
        bool flags = ImGuiPopupFlags_None;
        bool asset_update = false;

        if (ImGui::BeginPopupContextItem("Asset Creation", flags)) { 
            if (!selected_asset_info) {
                if (ImGui::Button("New Script")) {
                    CW::AssetManager::Get()->CreateScript("scripts/", "PlayerMovement");

                    ImGui::CloseCurrentPopup();
                }
                if (ImGui::BeginPopupContextItem("New_Material_Popup")){
                    static char buf1[48] = ""; 

                    ImGui::Text("Material Name: ");

                    ImGui::PushItemWidth(-FLT_EPSILON);
                    ImGui::InputText("Material", buf1, 32);
                    ImGui::PopItemWidth();

                    if(window->GetInputState(CW::KeyCode::RETURN)){
                        CW::Material mat = {};
                        mat.albedo_color = vec3s { 1.0f, 1.0f, 1.0f };
                        mat.albedo = CW::AssetManager::Get()->GetDefaultTextureIndex();
                        mat.normal_map = CW::AssetManager::Get()->GetDefaultTextureIndex();
                        mat.specular_map = CW::AssetManager::Get()->GetDefaultSpecularTextureIndex();
                        strcat(buf1, ".mat");
                        CW::AssetManager::Get()->CreateAndLoadMaterialAsset(buf1, mat);

                        asset_update = true;
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::EndPopup(); 
                }

                if (ImGui::Button("New Material"))
                    ImGui::OpenPopup("New_Material_Popup");
            } else {
                if (ImGui::Button("Delete")) {
                    selected_asset.asset_index = 0;
                    asset_update = true;
                    ImGui::CloseCurrentPopup();

                    switch (selected_asset_info->asset_type)
                    {
                        case AssetType::MATERIAL: {
                            CW::AssetManager::Get()->DeleteMaterial(selected_asset_info->asset_index);
                        } break;
                    }
                }
            }
                
            if (ImGui::Button("Close")) {
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }

        return asset_update;
    }
    void ApplicationView::RenderProjectSettings() {
        ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);
        if (ImGui::Begin("Project Settings", &show_project_settings, ImGuiWindowFlags_MenuBar))
        {
            ImGui::BeginGroup();
            ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));
            if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None))
            {
                if (ImGui::BeginTabItem("Settings"))
                {
                    CW::ProjectSpecification& spec = cogwheel->GetProjectManager()->GetCurrentProject()->specification;
                    ImGui::Text("Project name: %s", spec.project_name);

                    const char* items[] = { "Windowed", "Fullscreen" };
                    ImGui::Text("Resolution mode: ");
                    ImGui::PushItemWidth(-FLT_EPSILON);
                    ImGui::Combo("Resolution mode", (int*)&spec.resolution_mode, items, IM_ARRAYSIZE(items));
                    ImGui::PopItemWidth();

                    ImGuiInputTextFlags flags = ImGuiInputTextFlags_CharsDecimal;
                    if (spec.resolution_mode != CW::ResolutionMode::WINDOWED) flags |= ImGuiInputTextFlags_ReadOnly;
                    ImGui::Text("Resolution size: ");
                    ImGui::PushItemWidth(-FLT_EPSILON);
                    int size[2] = { (int) spec.windowed_size.raw[0], (int) spec.windowed_size.raw[1] };
                    ImGui::InputInt2("Resolution size", size, flags);
                    spec.windowed_size.x = (float) size[0];
                    spec.windowed_size.y = (float) size[1];
                    ImGui::PopItemWidth();

                    ImGui::Checkbox("Vsync", &spec.vsync);

                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Render Settings")) {
                    ImGui::Text("ID: 0123456789");
                    ImGui::EndTabItem();
                }
                ImGui::EndTabBar();
            }
            ImGui::EndChild();
            ImGui::EndGroup();
        }
        ImGui::End();
    }
    std::string ApplicationView::GetProjectFilePath(){
        std::string sFilePath;       
        //  CREATE FILE OBJECT INSTANCE   
        HRESULT f_SysHr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
        if (FAILED(f_SysHr))
            return "0";
        // CREATE FileOpenDialog OBJECT
        IFileOpenDialog* f_FileSystem;
        f_SysHr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&f_FileSystem));
        if (FAILED(f_SysHr)) {
            CoUninitialize();
            return "0";
        }
        //  SHOW OPEN FILE DIALOG WINDOW
        f_SysHr = f_FileSystem->Show(NULL);
        if (FAILED(f_SysHr)) {
            f_FileSystem->Release();
            CoUninitialize();
            return "0";
        }
        //  RETRIEVE FILE NAME FROM THE SELECTED ITEM
        IShellItem* f_Files;
        f_SysHr = f_FileSystem->GetResult(&f_Files);
        if (FAILED(f_SysHr)) {
            f_FileSystem->Release();
            CoUninitialize();
            return "0";
        }
        //  STORE AND CONVERT THE FILE NAME
        PWSTR f_Path;
        f_SysHr = f_Files->GetDisplayName(SIGDN_FILESYSPATH, &f_Path);
        if (FAILED(f_SysHr)) {
            f_Files->Release();
            f_FileSystem->Release();
            CoUninitialize();
            return "0";
        }  
        //  FORMAT AND STORE THE FILE PATH
        std::wstring path(f_Path);
        std::string c(path.begin(), path.end());
        sFilePath = c;

        //  SUCCESS, CLEAN UP
        CoTaskMemFree(f_Path);
        f_Files->Release();
        f_FileSystem->Release();
        CoUninitialize();
        std::replace(sFilePath.begin(), sFilePath.end(), '\\', '/');
        return sFilePath;            
                
    }
    bool ApplicationView::CheckNameConflict(char *name){
        CW::Scene& active_scene = cogwheel->GetSceneManager()->GetActiveScene();
        for(int i = 0; i < active_scene.game_objects.size(); i++){
            CW::GameObject game_object = *std::next(active_scene.game_objects.begin(), i);
            if(strcmp(game_object.GetComponent<CW::Transform>().name, name) == 0)
                return true;
        }
        return false;
    }
    char* ApplicationView::GetSubDirectory(char *dir) {
        char *last_slash = strchr(dir, '/');
        if (last_slash != 0) {
            char buffer[256] = {};
            strncpy(buffer, dir, strlen(dir) - strlen(last_slash));
            return buffer;
        } else {
            return "";
        }
    }
}