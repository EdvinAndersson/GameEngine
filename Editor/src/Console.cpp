#include "Console.h"

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "imgui/backends/imgui_impl_win32.h"

namespace CWEditor {

    static Console *console;

    Console::Console() {
        console = this;
    }

    void Console::Render() {
        if (ImGui::Button("Clear")) {
            log_entry_count = 0;
        }
        ImGui::SameLine();
        const char* items[] = { "Normal", "Warning", "Error" };
        ImGui::PushItemWidth(-FLT_EPSILON);
        ImGui::Combo("Log Level", &active_log_level, items, IM_ARRAYSIZE(items));
        ImGui::PopItemWidth();

        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImGui::GetStyleColorVec4(ImGuiCol_FrameBg));
        if (ImGui::BeginChild("ChildL", ImVec2(0, 0), ImGuiChildFlags_None, ImGuiWindowFlags_HorizontalScrollbar)) {

            for (int i = 0; i < log_entry_count; i++) {
                LogEntry entry = log_entries[i];

                if (entry.log_level < active_log_level) 
                    continue;

                ImVec4 color = {};
                char *log_info = "";
                switch (entry.log_level) {
                    case LogLevel::LOG_NORMAL: {
                        color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
                        log_info = "[LOG]";
                    } break;
                    case LogLevel::LOG_WARNING: {
                        color = ImVec4(255.0f / 255.0f, 180.0f / 255.0f, 61.0f / 255.0f, 1.0f);
                        log_info = "[WARNING]";
                    } break;
                    case LogLevel::LOG_ERROR: {
                        color = ImVec4(1.0f, 0.2f, 0.1f, 1.0f);
                        log_info = "[ERROR]";
                    } break;
                }

                ImGui::TextColored(color, "%s %s", log_info, entry.text);
            }

            if (console->scroll_to_bottom)
                ImGui::SetScrollHereY(0.0f);

            console->scroll_to_bottom = false;

            ImGui::EndChild();
            ImGui::PopStyleColor();
        }
    }

    void Console::Log(char *text) {
        Log(LogLevel::LOG_NORMAL, text);
    }
    void Console::Log(LogLevel log_level, char *text) {
        LogEntry entry;
        entry.log_level = log_level;
        strcpy(entry.text, text);

        console->log_entries[console->log_entry_count++] = entry;
        console->scroll_to_bottom = true;
    }
}