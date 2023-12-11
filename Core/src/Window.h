#pragma once
#include <Windows.h>
#include "GLMath.h"
#include <windowsx.h>
#include <stdio.h>
#include <GL/GL.h>
#include "GLFunc.h"

#ifndef UNICODE
#define UNICODE
#endif

namespace CW {

	enum KeyCode {
		MOUSE_BUTTON_1 = 0x01,
		MOUSE_BUTTON_2 = 0x02,
		MOUSE_BUTTON_3 = 0x03,
		BACKSPACE = 0x08,
		TAB = 0x09,
		RETURN = 0x0D,
		SHIFT = 0x10,
		CONTROL = 0x11,
		CAPITAL = 0x14,
		ESCAPE = 0x1B,
		SPACE = 0x20,
		LEFT = 0x25,
		UP = 0x26,
		RIGHT = 0x27,
		DOWN = 0x28,
		ZERO = 0x30,
		ONE = 0x31,
		TWO = 0x32,
		THREE = 0x33,
		FOUR = 0x34,
		FIVE = 0x35,
		SIX = 0x36,
		SEVEN = 0x37,
		EIGHT = 0x38,
		NINE = 0x39,
		A = 0x41,
		B = 0x42,
		C = 0x43,
		D = 0x44,
		E = 0x45,
		F = 0x46,
		G = 0x47,
		H = 0x48,
		I = 0x49,
		J = 0x4A,
		K = 0x4B,
		L = 0x4C,
		M = 0x4D,
		N = 0x4E,
		O = 0x4F,
		P = 0x50,
		Q = 0x51,
		R = 0x52,
		S = 0x53,
		T = 0x54,
		U = 0x55,
		V = 0x56,
		W = 0x57,
		X = 0x58,
		Y = 0x59,
		Z = 0x5A,
		MULTIPLY = 0x6A,
		ADD = 0x6B,
		SEPERATOR = 0x6C,
		SUBTRACT = 0x6D,
		DIVIDE = 0x6F,
		LSHIFT = 0xA0,
		RSHIFT = 0xA1,
		F1 = 0x70,
		F2 = 0x71,
		F3 = 0x72,
		F4 = 0x73,
		F5 = 0x74,
		F6 = 0x75,
		F7 = 0x76,
		F8 = 0x77,
		F9 = 0x78,
		F10 = 0x79,
		F11 = 0x7A,
		F12 = 0x7B,
	};

	#define Assert(cond) do { if (!(cond)) __debugbreak(); } while (0)

	class Window {
	public:
		int Init(const wchar_t *title, int width, int height);
		void PollEvents();
		int WindowCallback(UINT uMsg, WPARAM wParam, LPARAM lParam);
		void Destroy();
		void WinSwapBuffers();

		bool GetInputState(KeyCode keycode);

		void WinSetFullscreen(bool fullscreen);
		void WinSetMousePosition(int x, int y);
		void WinShowCursor(bool show);
		void EnableVSync(bool enable);
		inline HWND GetHandle() { return hwnd; }
		inline int GetWidth() { return window_width; }
		inline int GetHeight() { return window_height; }
		int GetMousePositionX();
		int GetMousePositionY();
		double GetTime();
		inline double GetCpuFrequency() { return cpu_frequency; }
		inline bool IsFullscreen() { return is_fullscreen; }

	private:
		void CreateOpenGLContext();
		void LoadOpenGLFunctions();
		void GetWglFunctions();
		void FatalError(const char* message);
		int StringsAreEqual(const char* src, const char* dst, size_t dstlen);

		HWND hwnd;
		HDC hdc;

		PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = NULL;
		PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = NULL;

		HGLRC renderingContext;
		WINDOWPLACEMENT g_wpPrev = { sizeof(g_wpPrev) };

		bool is_fullscreen;
		double process_start_time, cpu_frequency;
		int window_width, window_height;
		int mouse_position_x, mouse_position_y;
	};

	Window *WinGetWindowInstance();
}