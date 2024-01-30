#include "Window.h"
#include "CWAssert.h"

#pragma comment (lib, "opengl32.lib")

#undef X
#define X(proc, name) proc name;
GLFUNCTIONS

namespace CW {

	Window *window_instance;

	LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
		if (window_instance->GetImGUIWindowsProcHandler())
			if (window_instance->GetImGUIWindowsProcHandler()(hwnd, uMsg, wParam, lParam)) return true;

		if (window_instance->WindowCallback(uMsg, wParam, lParam)) return true;

		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	int Window::Init(const wchar_t *title, int width, int height) {
		window_instance = this;

		const wchar_t CLASS_NAME[] = L" My Window Class";

		WNDCLASS wc = {};
		wc.lpfnWndProc = WindowProc;
		wc.hInstance = GetModuleHandle(0);
		wc.lpszClassName = CLASS_NAME;
		wc.style = CS_OWNDC;
		wc.hCursor = LoadCursor(0, IDC_ARROW);

		if (!RegisterClass(&wc))
			return 1;

		int xPos = GetSystemMetrics(SM_CXSCREEN) / 2 - width / 2;
		int yPos = GetSystemMetrics(SM_CYSCREEN) / 2 - height / 2;

		hwnd = CreateWindowEx(
			0,
			CLASS_NAME,
			title,
			WS_OVERLAPPEDWINDOW,

			xPos, yPos, width, height, //Size and position

			NULL, // Parent window
			NULL, // Menu
			wc.hInstance,
			NULL);

		if (hwnd == NULL)
			return 1;


		ShowWindow(hwnd, 1);

		hdc = GetDC(hwnd);

		CreateOpenGLContext();
		LoadOpenGLFunctions();

		LARGE_INTEGER temp_cpu_frequency;
		QueryPerformanceFrequency(&temp_cpu_frequency);
		cpu_frequency = (double) temp_cpu_frequency.QuadPart;

		LARGE_INTEGER temp;
		QueryPerformanceCounter(&temp);
		process_start_time = (double) temp.QuadPart / cpu_frequency;

		EnableVSync(true);

		return 0;
	}
	void Window::Destroy() {
		DestroyWindow(hwnd);
	}

	void Window::PollEvents() {
		MSG msg = {};
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	void Window::WinSwapBuffers() {
		SwapBuffers(hdc);
	}

	int Window::WindowCallback(UINT uMsg, WPARAM wParam, LPARAM lParam) {
		switch (uMsg)
		{
			case WM_DESTROY:
			{
				if (on_window_event) {
					on_window_event(WindowEventType::EVENT_WINDOW_CLOSE, &WindowEventClose());
				}

				wglDeleteContext(renderingContext);
				PostQuitMessage(0);
				return 1;
			}
			case WM_SIZE:
			{
				window_width = LOWORD(lParam);
				window_height = HIWORD(lParam);

				if (on_window_event) {
					WindowEventResize e = {window_width, window_height};
					on_window_event(WindowEventType::EVENT_WINDOW_RESIZE, &e);
				}

				return 1;
			}
			case WM_MOUSEWHEEL:
			{
				int scroll = GET_Y_LPARAM(wParam);

				
				return 1;
			}
			case WM_MOUSEMOVE:
			{
				int new_mouse_position_x = GET_X_LPARAM(lParam);
				int new_mouse_position_y = GET_Y_LPARAM(lParam);

				//if (onMouseCallback)
				//	onMouseCallback(new_mouse_position_x, new_mouse_position_y);

				mouse_position_x = new_mouse_position_x;
				mouse_position_y = new_mouse_position_y;

				return 1;
			}
			case WM_KEYDOWN:
			{
				if (on_window_event) {
					WindowEventKeyDown e = { (KeyCode) wParam };
					on_window_event(WindowEventType::EVENT_WINDOW_KEYDOWN, &e);
				}

				if ((lParam & (1 << 30)) == 0) {
					if (on_window_event) {
						WindowEventKeyPressed e = { (KeyCode) wParam };
						on_window_event(WindowEventType::EVENT_WINDOW_KEYPRESSED, &e);
					}
				}

				return 1;
			}
			case WM_KEYUP:
			{
				if (on_window_event) {
					WindowEventKeyUp e = { (KeyCode) wParam };
					on_window_event(WindowEventType::EVENT_WINDOW_KEYUP, &e);
				}
				return 1;
			}
		}
		return 0;
	}
	void Window::EnableVSync(bool enable) {
		if (wglSwapIntervalEXT)
			wglSwapIntervalEXT(enable);
	}
	void Window::WinSetMousePosition(int x, int y) {
		POINT *pt = new POINT();
		pt->x = x;
		pt->y = y;

		ClientToScreen(hwnd, pt);
		SetCursorPos(pt->x, pt->y);

		delete pt;
	}
	void Window::WinSetFullscreen(bool fullscreen) {
		DWORD dwStyle = GetWindowLong(hwnd, GWL_STYLE);

		if (fullscreen && dwStyle & WS_OVERLAPPEDWINDOW) {
			MONITORINFO mi = { sizeof(mi) };
			if (GetWindowPlacement(hwnd, &g_wpPrev) &&
				GetMonitorInfo(MonitorFromWindow(hwnd, MONITOR_DEFAULTTOPRIMARY), &mi)) {
				SetWindowLong(hwnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);

				SetWindowPos(hwnd, HWND_TOP,
					mi.rcMonitor.left, mi.rcMonitor.top,
					mi.rcMonitor.right - mi.rcMonitor.left,
					mi.rcMonitor.bottom - mi.rcMonitor.top,
					SWP_NOOWNERZORDER | SWP_FRAMECHANGED);

				is_fullscreen = true;
			}
		} else if (!fullscreen && !(dwStyle & WS_OVERLAPPEDWINDOW)) {
			SetWindowLong(hwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);

			SetWindowPlacement(hwnd, &g_wpPrev);
			SetWindowPos(hwnd, NULL, 0, 0, 0, 0,
				SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER |
				SWP_NOOWNERZORDER | SWP_FRAMECHANGED);

			is_fullscreen = false;
		}
	}
	void Window::WinShowCursor(bool show) {
		ShowCursor(show);
	}
	void Window::WinSetTitle(char *title) {
		size_t newsize = strlen(title) + 1;
		wchar_t* wcstring = new wchar_t[newsize];
		size_t convertedChars = 0;
		mbstowcs_s(&convertedChars, wcstring, newsize, title, _TRUNCATE);

		SetWindowText(hwnd, wcstring);

		delete []wcstring;
	}
	void Window::WinSetSize(int width, int height) {
		int xPos = GetSystemMetrics(SM_CXSCREEN) / 2 - width / 2;
		int yPos = GetSystemMetrics(SM_CYSCREEN) / 2 - height / 2;

		SetWindowPos(hwnd, 0, xPos, yPos, width, height, SWP_FRAMECHANGED);
	}
	int Window::GetMousePositionX() { 
		return mouse_position_x;
	}
	int Window::GetMousePositionY() {
		return mouse_position_y;
	}
	double Window::GetTime() {
		LARGE_INTEGER current_counter;
		QueryPerformanceCounter(&current_counter);

		return (double)current_counter.QuadPart / cpu_frequency;
	}

	bool Window::GetInputState(KeyCode keycode) {
		return GetKeyState(keycode) & 0x8000;
	}

	void Window::CreateOpenGLContext() {
		GetWglFunctions();

		{ // set pixel format for OpenGL context
			int attrib[] =
			{
				WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
				WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
				WGL_DOUBLE_BUFFER_ARB,  GL_TRUE,
				WGL_PIXEL_TYPE_ARB,     WGL_TYPE_RGBA_ARB,
				WGL_COLOR_BITS_ARB,     24,
				WGL_DEPTH_BITS_ARB,     24,
				WGL_STENCIL_BITS_ARB,   8,
				//WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB, GL_TRUE,
				//WGL_SAMPLE_BUFFERS_ARB, 1,
				//WGL_SAMPLES_ARB,        4, // 4x MSAA
				0,
			};

			int format;
			UINT formats;
			if (!wglChoosePixelFormatARB(hdc, attrib, NULL, 1, &format, &formats) || formats == 0)
			{
				FatalError("OpenGL does not support required pixel format!");
			}

			PIXELFORMATDESCRIPTOR desc;
			desc.nSize = sizeof(desc);
			int ok = DescribePixelFormat(hdc, format, sizeof(desc), &desc);
			CW_ASSERT(ok, "Failed to describe OpenGL pixel format");

			if (!SetPixelFormat(hdc, format, &desc))
			{
				FatalError("Cannot set OpenGL selected pixel format!");
			}
		}

		// create modern OpenGL context
		int attrib[] =
		{
			WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
			WGL_CONTEXT_MINOR_VERSION_ARB, 5,
			WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
	#ifndef NDEBUG
			// ask for debug context for non "Release" builds
			// this is so we can enable debug callback
			WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB,
	#endif
			0,
		};

		HGLRC rc = wglCreateContextAttribsARB(hdc, NULL, attrib);
		if (!rc)
		{
			FatalError("Cannot create modern OpenGL context! OpenGL version 4.5 not supported?");
		}

		BOOL ok = wglMakeCurrent(hdc, rc);
		CW_ASSERT(ok, "Failed to make current OpenGL context");

		printf("OPENGL VERSION: %s\n", (char *)glGetString(GL_VERSION));
	}

	void Window::LoadOpenGLFunctions() {
		#undef X
		#define X(proc, name) name=(proc)wglGetProcAddress(#name);
		GLFUNCTIONS
	}

	void Window::GetWglFunctions()
	{
		// to get WGL functions we need valid GL context, so create dummy window for dummy GL contetx
		HWND dummy = CreateWindowExW(
			0, L"STATIC", L"DummyWindow", WS_OVERLAPPED,
			CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
			NULL, NULL, NULL, NULL);
		CW_ASSERT(dummy, "Failed to create dummy window");

		HDC dc = GetDC(dummy);
		CW_ASSERT(dc, "Failed to get device context for dummy window");

		PIXELFORMATDESCRIPTOR desc;
		desc.nSize = sizeof(desc);
		desc.nVersion = 1;
		desc.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		desc.iPixelType = PFD_TYPE_RGBA;
		desc.cColorBits = 24;
		
		int format = ChoosePixelFormat(dc, &desc);
		if (!format)
		{
			FatalError("Cannot choose OpenGL pixel format for dummy window!");
		}

		int ok = DescribePixelFormat(dc, format, sizeof(desc), &desc);
		CW_ASSERT(ok, "Failed to describe OpenGL pixel format");

		// reason to create dummy window is that SetPixelFormat can be called only once for the window
		if (!SetPixelFormat(dc, format, &desc))
		{
			FatalError("Cannot set OpenGL pixel format for dummy window!");
		}

		HGLRC rc = wglCreateContext(dc);
		CW_ASSERT(rc, "Failed to create OpenGL context for dummy window");

		ok = wglMakeCurrent(dc, rc);
		CW_ASSERT(ok, "Failed to make current OpenGL context for dummy window");

		// https://www.khronos.org/registry/OpenGL/extensions/ARB/WGL_ARB_extensions_string.txt
		PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsStringARB = (PFNWGLGETEXTENSIONSSTRINGARBPROC) wglGetProcAddress("wglGetExtensionsStringARB");
		if (!wglGetExtensionsStringARB)
		{
			FatalError("OpenGL does not support WGL_ARB_extensions_string extension!");
		}

		const char* ext = wglGetExtensionsStringARB(dc);
		CW_ASSERT(ext, "Failed to get OpenGL WGL extension string");

		const char* start = ext;
		for (;;)
		{
			while (*ext != 0 && *ext != ' ')
			{
				ext++;
			}

			size_t length = ext - start;
			if (StringsAreEqual("WGL_ARB_pixel_format", start, length))
			{
				// https://www.khronos.org/registry/OpenGL/extensions/ARB/WGL_ARB_pixel_format.txt
				wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC) wglGetProcAddress("wglChoosePixelFormatARB");
			}
			else if (StringsAreEqual("WGL_ARB_create_context", start, length))
			{
				// https://www.khronos.org/registry/OpenGL/extensions/ARB/WGL_ARB_create_context.txt
				wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC) wglGetProcAddress("wglCreateContextAttribsARB");
			}

			if (*ext == 0)
			{
				break;
			}

			ext++;
			start = ext;
		}

		if (!wglChoosePixelFormatARB || !wglCreateContextAttribsARB)
		{
			FatalError("OpenGL does not support required WGL extensions for modern context!");
		}

		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(rc);
		ReleaseDC(dummy, dc);
		DestroyWindow(dummy);
	}
	void Window::FatalError(const char* message)
	{
		MessageBoxA(NULL, message, "Error", MB_ICONEXCLAMATION);
		ExitProcess(0);
	}

	int Window::StringsAreEqual(const char* src, const char* dst, size_t dstlen)
	{
		while (*src && dstlen-- && *dst)
		{
			if (*src++ != *dst++)
			{
				return 0;
			}
		}

		return (dstlen && *src == *dst) || (!dstlen && *src == 0);
	}

	Window *WinGetWindowInstance() {
		return window_instance;
	}
}