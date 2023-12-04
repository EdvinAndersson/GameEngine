#include <stdio.h>

#include "Core/src/Window.h"
#include "Core/src/rendering/Renderer3D.h"

GL::Window *window;
bool g_running = true;

int main() {
    window = new GL::Window();
    window->Init(L"Game Engine", 1024, 600);
    window->EnableVSync(true);
    window->SetOnQuitCallback([]() { g_running = false; });

    GL::R3D_Init(window);

    while (g_running) {
        if (window->GetInputState(GL::KeyCode::ESCAPE)){
            g_running = false;
            window->Destroy();
            return 0;
        }
        GL::R3D_Clear(Vec4{0.003f, 0.003f, 0.005f, 1.0f});

        GL::R3D_RenderCube(Vec3{0.0f, 0.0f, -4.0f }, Vec3{1,1,1}, Vec3{0.5f, 0.5f, 0.5f});

        window->WinSwapBuffers();
        window->PollEvents();
    }

    return 0;
}