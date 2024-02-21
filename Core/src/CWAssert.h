#pragma once

#include <Windows.h>
#include <stdio.h>

#define CW_ASSERT(expr, msg) if (!(expr)) { __CW_ASSERT(#expr, __FILE__, __LINE__, msg); DebugBreak(); }

inline void __CW_ASSERT(const char* expr_str, const char* file, int line, const char* msg)
{
    char buffer[1024] = {};
    sprintf_s(buffer, 1024, "Assert failed: %s\nExpected: %s\nSource: %s, line %i\n", msg, expr_str, file, line);

    MessageBoxA(0, buffer, "Assertion", MB_OK | MB_ICONERROR);
}