#pragma once

namespace CWEditor {

    #define MAX_LOG_ENTRIES 256

    enum LogLevel {
        LOG_NORMAL,
        LOG_WARNING,
        LOG_ERROR
    };

    struct LogEntry {
        char text[256];
        LogLevel log_level;
    };

    class Console {
        public:
            Console();
            void Render();

            static void Log(char *text);
            static void Log(LogLevel log_level, char *text);
        private:
            bool scroll_to_bottom = true;
            LogEntry log_entries[MAX_LOG_ENTRIES];
            int log_entry_count = 0;
            int active_log_level = 0;
    };
}