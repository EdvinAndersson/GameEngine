#include <stdio.h>

#include "Core/src/Cogwheel.h"
#include "Core/src/rendering/Renderer3D.h"
#include "Core/src/event/EventManager.h"
#include "Core/src/ecs/GameObject.h"

#include "vendor/imgui/imgui.h"
#include "vendor/imgui/backends/imgui_impl_opengl3.h"
#include "vendor/imgui/backends/imgui_impl_win32.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int main() {

    CW::Cogwheel *cogwheel = new CW::Cogwheel();
    cogwheel->Init();
    cogwheel->GetWindow()->SetImGUIWindowsProcHandler(ImGui_ImplWin32_WndProcHandler);

    for (int i = 0; i < 3; i++) {
        CW::GameObject obj = CW::GameObject::Instantiate();
        CW::Transform& transform = obj.GetComponent<CW::Transform>();
        transform.position = Vec3 {-1.5f + i*1.5f, 0, -4.0f };
    }

    CW::Model sword;
    sword.Load("Editor/res/Shield.obj");

    //Initialize ImGUI
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;    // Enable Gamepad Controls
    ImGui::StyleColorsDark();
    ImGui_ImplWin32_InitForOpenGL(cogwheel->GetWindow()->GetHandle());
    ImGui_ImplOpenGL3_Init();

    cogwheel->GetSceneManager()->CreateNewScene("Test scene");

    while (cogwheel->IsRunning()) {
        glEnable(GL_FRAMEBUFFER_SRGB);
        CW::R3D_Clear(Vec4 {0,0,0,1} );

        cogwheel->Update();
        
        static float angle = 0;
        angle += 0.2f;

        CW::R3D_RenderModel(&sword, Vec3 {0,-1.0f,-3}, {0.1f,0.1f,0.1f}, QuatAngleAxis(angle, Vec3{0,1,0}));

        static float rotation;
        rotation += 0.001f;

        Mat4 view = Mat4Identity();
        view = Mat4Rotate(view, rotation, Vec3{0,1,0} );

        CW::R3D_SetViewModel(view);

        glDisable(GL_FRAMEBUFFER_SRGB);
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Debug View");

        if (ImGui::Button("Save project")) {
            cogwheel->GetProjectManager()->SaveProject("Editor/res/projects/");
        }
        if (ImGui::Button("Load project 1")) {
            cogwheel->GetProjectManager()->LoadProject("Editor/res/projects/Unnamed Project.proj");
        }
        if (ImGui::Button("Load project 2")) {
            cogwheel->GetProjectManager()->LoadProject("Editor/res/projects/TestProject.proj");
        }

        ImGui::End();

        //ImGui::ShowDemoWindow();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        cogwheel->SwapBuffersAndPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    return 0;
}