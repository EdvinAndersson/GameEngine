#include <stdio.h>

#include "Core/src/Cogwheel.h"
#include "Core/src/rendering/Renderer3D.h"
#include "Core/src/event/EventManager.h"

#include "vendor/imgui/imgui.h"
#include "vendor/imgui/backends/imgui_impl_opengl3.h"
#include "vendor/imgui/backends/imgui_impl_win32.h"

int main() {

    CW::Cogwheel *cogwheel = new CW::Cogwheel();
    cogwheel->Init(L"Cogwheel Editor", 1920, 1080);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;    // Enable Gamepad Controls

    ImGui::StyleColorsDark();
    ImGui_ImplWin32_InitForOpenGL(cogwheel->GetWindow()->GetHandle());
    ImGui_ImplOpenGL3_Init();


    CW::Model model;
    CW::LoadModel(&model, "Editor/res/Sponza/sponza.obj");

    while (cogwheel->IsRunning()) {
        CW::R3D_Clear(Vec4 {0,0,0,1} );
        CW::R3D_RenderCube(Vec3{1.5f,0,-2}, Vec3{1,1,1}, Vec3{1,0,0});
        CW::R3D_RenderCube(Vec3{-1.5f,0,-2}, Vec3{1,1,1}, Vec3{0,0,1});

        CW::R3D_RenderModel(&model, Vec3 {0,0,0}, Vec3 {0.1f, 0.1f, 0.1f});

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        ImGui::ShowDemoWindow();

        if (ImGui::TreeNode("Collapsing Headers"))
        {
            static bool closable_group = true;
            ImGui::Checkbox("Show 2nd header", &closable_group);
            if (ImGui::CollapsingHeader("Header", ImGuiTreeNodeFlags_None))
            {
                ImGui::Text("IsItemHovered: %d", ImGui::IsItemHovered());
                for (int i = 0; i < 5; i++)
                    ImGui::Text("Some content %d", i);
            }
            if (ImGui::CollapsingHeader("Header with a close button", &closable_group))
            {
                ImGui::Text("IsItemHovered: %d", ImGui::IsItemHovered());
                for (int i = 0; i < 5; i++)
                    ImGui::Text("More content %d", i);
            }
            
            ImGui::TreePop();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        cogwheel->Update();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    return 0;
}