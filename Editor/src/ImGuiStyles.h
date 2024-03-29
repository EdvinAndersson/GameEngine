#pragma once

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "imgui/backends/imgui_impl_win32.h"

namespace CWEditor {

    inline void ImGuiStyleGray(){
        ImVec4* colors = ImGui::GetStyle().Colors;
        colors[ImGuiCol_Text]                   = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        colors[ImGuiCol_TextDisabled]           = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
        colors[ImGuiCol_WindowBg]               = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
        colors[ImGuiCol_ChildBg]                = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_PopupBg]                = ImVec4(0.09f, 0.09f, 0.09f, 0.92f);
        colors[ImGuiCol_Border]                 = ImVec4(0.00f, 0.00f, 0.00f, 0.29f);
        colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.24f);
        colors[ImGuiCol_FrameBg]                = ImVec4(0.17f, 0.17f, 0.17f, 1.00f);
        colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
        colors[ImGuiCol_FrameBgActive]          = ImVec4(0.17f, 0.17f, 0.18f, 1.00f);
        colors[ImGuiCol_TitleBg]                = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
        colors[ImGuiCol_TitleBgActive]          = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.17f, 0.17f, 0.19f, 1.00f);
        colors[ImGuiCol_MenuBarBg]              = ImVec4(0.35f, 0.35f, 0.38f, 1.00f);
        colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.11f, 0.11f, 0.13f, 1.00f);
        colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.20f, 0.20f, 0.22f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.31f, 0.31f, 0.34f, 1.00f);
        colors[ImGuiCol_CheckMark]              = ImVec4(0.96f, 0.95f, 0.99f, 1.00f);
        colors[ImGuiCol_SliderGrab]             = ImVec4(0.56f, 0.55f, 0.60f, 0.54f);
        colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.92f, 0.90f, 0.96f, 0.54f);
        colors[ImGuiCol_Button]                 = ImVec4(0.19f, 0.19f, 0.21f, 1.00f);
        colors[ImGuiCol_ButtonHovered]          = ImVec4(0.22f, 0.22f, 0.24f, 1.00f);
        colors[ImGuiCol_ButtonActive]           = ImVec4(0.28f, 0.28f, 0.30f, 1.00f);
        colors[ImGuiCol_Header]                 = ImVec4(0.21f, 0.21f, 0.23f, 1.00f);
        colors[ImGuiCol_HeaderHovered]          = ImVec4(0.24f, 0.25f, 0.27f, 1.00f);
        colors[ImGuiCol_HeaderActive]           = ImVec4(0.18f, 0.18f, 0.20f, 1.00f);
        colors[ImGuiCol_Separator]              = ImVec4(0.12f, 0.12f, 0.13f, 1.00f);
        colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.11f, 0.11f, 0.13f, 1.00f);
        colors[ImGuiCol_SeparatorActive]        = ImVec4(0.12f, 0.12f, 0.14f, 1.00f);
        colors[ImGuiCol_ResizeGrip]             = ImVec4(0.40f, 0.39f, 0.43f, 0.29f);
        colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.91f, 0.91f, 0.91f, 0.29f);
        colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.81f, 0.81f, 0.82f, 0.29f);
        colors[ImGuiCol_Tab]                    = ImVec4(0.18f, 0.18f, 0.20f, 1.00f);
        colors[ImGuiCol_TabHovered]             = ImVec4(0.29f, 0.29f, 0.32f, 1.00f);
        colors[ImGuiCol_TabActive]              = ImVec4(0.24f, 0.25f, 0.28f, 1.00f);
        colors[ImGuiCol_TabUnfocused]           = ImVec4(0.21f, 0.21f, 0.23f, 1.00f);
        colors[ImGuiCol_TabUnfocusedActive]     = ImVec4(0.20f, 0.20f, 0.22f, 1.00f);
        colors[ImGuiCol_DockingPreview]         = ImVec4(0.62f, 0.61f, 0.65f, 1.00f);
        colors[ImGuiCol_DockingEmptyBg]         = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
        colors[ImGuiCol_PlotLines]              = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
        colors[ImGuiCol_PlotLinesHovered]       = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
        colors[ImGuiCol_PlotHistogram]          = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
        colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
        colors[ImGuiCol_TableHeaderBg]          = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
        colors[ImGuiCol_TableBorderStrong]      = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
        colors[ImGuiCol_TableBorderLight]       = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
        colors[ImGuiCol_TableRowBg]             = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_TableRowBgAlt]          = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
        colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
        colors[ImGuiCol_DragDropTarget]         = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
        colors[ImGuiCol_NavHighlight]           = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);



        auto &style = ImGui::GetStyle();
        style.TabRounding = 4;
        style.ScrollbarRounding = 9;
        style.WindowRounding = 7;
        style.GrabRounding = 3;
        style.FrameRounding = 3;
        style.PopupRounding = 4;
        style.ChildRounding = 4;
    }

    inline void ImGuiStyleBlue(){
        auto &colors = ImGui::GetStyle().Colors;
        colors[ImGuiCol_WindowBg] = ImVec4{0.1f, 0.1f, 0.13f, 1.0f};
        colors[ImGuiCol_MenuBarBg] = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};
        colors[ImGuiCol_Border] = ImVec4{0.44f, 0.37f, 0.61f, 0.29f};
        colors[ImGuiCol_BorderShadow] = ImVec4{0.0f, 0.0f, 0.0f, 0.24f};
        colors[ImGuiCol_Text] = ImVec4{1.0f, 1.0f, 1.0f, 1.0f};
        colors[ImGuiCol_TextDisabled] = ImVec4{0.5f, 0.5f, 0.5f, 1.0f};
        colors[ImGuiCol_Header] = ImVec4{0.13f, 0.13f, 0.17, 1.0f};
        colors[ImGuiCol_HeaderHovered] = ImVec4{0.19f, 0.2f, 0.25f, 1.0f};
        colors[ImGuiCol_HeaderActive] = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};
        colors[ImGuiCol_Button] = ImVec4{0.13f, 0.13f, 0.17, 1.0f};
        colors[ImGuiCol_ButtonHovered] = ImVec4{0.19f, 0.2f, 0.25f, 1.0f};
        colors[ImGuiCol_ButtonActive] = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};
        colors[ImGuiCol_CheckMark] = ImVec4{0.74f, 0.58f, 0.98f, 1.0f};
        colors[ImGuiCol_PopupBg] = ImVec4{0.1f, 0.1f, 0.13f, 0.92f};
        colors[ImGuiCol_SliderGrab] = ImVec4{0.44f, 0.37f, 0.61f, 0.54f};
        colors[ImGuiCol_SliderGrabActive] = ImVec4{0.74f, 0.58f, 0.98f, 0.54f};
        colors[ImGuiCol_FrameBg] = ImVec4{0.13f, 0.13, 0.17, 1.0f};
        colors[ImGuiCol_FrameBgHovered] = ImVec4{0.19f, 0.2f, 0.25f, 1.0f};
        colors[ImGuiCol_FrameBgActive] = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};
        colors[ImGuiCol_Tab] = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};
        colors[ImGuiCol_TabHovered] = ImVec4{0.24, 0.24f, 0.32f, 1.0f};
        colors[ImGuiCol_TabActive] = ImVec4{0.2f, 0.22f, 0.27f, 1.0f};
        colors[ImGuiCol_TabUnfocused] = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};
        colors[ImGuiCol_TitleBg] = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};
        colors[ImGuiCol_TitleBgActive] = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};
        colors[ImGuiCol_ScrollbarBg] = ImVec4{0.1f, 0.1f, 0.13f, 1.0f};
        colors[ImGuiCol_ScrollbarGrab] = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4{0.19f, 0.2f, 0.25f, 1.0f};
        colors[ImGuiCol_ScrollbarGrabActive] = ImVec4{0.24f, 0.24f, 0.32f, 1.0f};
        colors[ImGuiCol_Separator] = ImVec4{0.44f, 0.37f, 0.61f, 1.0f};
        colors[ImGuiCol_SeparatorHovered] = ImVec4{0.74f, 0.58f, 0.98f, 1.0f};
        colors[ImGuiCol_SeparatorActive] = ImVec4{0.84f, 0.58f, 1.0f, 1.0f};
        colors[ImGuiCol_ResizeGrip] = ImVec4{0.44f, 0.37f, 0.61f, 0.29f};
        colors[ImGuiCol_ResizeGripHovered] = ImVec4{0.74f, 0.58f, 0.98f, 0.29f};
        colors[ImGuiCol_ResizeGripActive] = ImVec4{0.84f, 0.58f, 1.0f, 0.29f};
        colors[ImGuiCol_DockingPreview] = ImVec4{0.44f, 0.37f, 0.61f, 1.0f};

        auto &style = ImGui::GetStyle();
        style.TabRounding = 4;
        style.ScrollbarRounding = 9;
        style.WindowRounding = 7;
        style.GrabRounding = 3;
        style.FrameRounding = 3;
        style.PopupRounding = 4;
        style.ChildRounding = 4;
    }
}