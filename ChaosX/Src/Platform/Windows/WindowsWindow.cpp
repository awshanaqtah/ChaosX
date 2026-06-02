#include "cspch.h"
#include "Platform/Windows/WindowsWindow.h"

#include "ChaosX/Log.h"
#include "ChaosX/Event/ApplicationEvent.h"
#include "ChaosX/Event/KeyEvent.h"
#include "ChaosX/Event/MouseEvent.h"

namespace ChaosX
{
	static const char* s_WindowClassName = "ChaosXWindowClass";
	static bool s_ClassRegistered = false;

	Window* Window::Create(const WindowProps& props)
	{
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		CS_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

		HINSTANCE hInstance = GetModuleHandleA(nullptr);

		if (!s_ClassRegistered)
		{
			WNDCLASSEXA wc = {};
			wc.cbSize = sizeof(WNDCLASSEXA);
			wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
			wc.lpfnWndProc = WindowProc;
			wc.hInstance = hInstance;
			wc.hCursor = LoadCursorW(nullptr, IDC_ARROW); // IDC_ARROW is an int resource (wide macro); LoadCursorW avoids the LPCSTR mismatch
			wc.lpszClassName = s_WindowClassName;
			RegisterClassExA(&wc);
			s_ClassRegistered = true;
		}

		// Size the window so the *client* area matches the requested size.
		RECT rect = { 0, 0, (LONG)props.Width, (LONG)props.Height };
		AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

		m_Hwnd = CreateWindowExA(
			0,
			s_WindowClassName,
			props.Title.c_str(),
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT,
			rect.right - rect.left, rect.bottom - rect.top,
			nullptr, nullptr, hInstance, nullptr);

		// Hand the static WindowProc a pointer to this window's data.
		SetWindowLongPtrA(m_Hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(&m_Data));

		ShowWindow(m_Hwnd, SW_SHOW);
	}

	void WindowsWindow::Shutdown()
	{
		if (m_Hwnd)
			DestroyWindow(m_Hwnd);
		m_Hwnd = nullptr;
	}

	void WindowsWindow::OnUpdate()
	{
		// Non-blocking: drain everything currently queued, then return so the
		// run loop keeps ticking.
		MSG msg;
		while (PeekMessageA(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}
	}

	LRESULT CALLBACK WindowsWindow::WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		WindowData* data = reinterpret_cast<WindowData*>(GetWindowLongPtrA(hWnd, GWLP_USERDATA));

		// Early creation/teardown messages arrive before the callback is wired up.
		if (!data || !data->EventCallback)
			return DefWindowProcA(hWnd, msg, wParam, lParam);

		switch (msg)
		{
			case WM_SIZE:
			{
				unsigned int width = LOWORD(lParam);
				unsigned int height = HIWORD(lParam);
				data->Width = width;
				data->Height = height;
				WindowResizeEvent e(width, height);
				data->EventCallback(e);
				return 0;
			}
			case WM_CLOSE:
			{
				// Don't destroy here; let Application decide via the event.
				WindowCloseEvent e;
				data->EventCallback(e);
				return 0;
			}
			case WM_MOUSEMOVE:
			{
				float x = (float)((short)LOWORD(lParam));
				float y = (float)((short)HIWORD(lParam));
				MouseMovedEvent e(x, y);
				data->EventCallback(e);
				return 0;
			}
			case WM_MOUSEWHEEL:
			{
				float yOffset = (float)((short)HIWORD(wParam)) / 120.0f; // WHEEL_DELTA
				MouseScrolledEvent e(0.0f, yOffset);
				data->EventCallback(e);
				return 0;
			}
			case WM_LBUTTONDOWN:
			case WM_RBUTTONDOWN:
			case WM_MBUTTONDOWN:
			{
				int button = (msg == WM_LBUTTONDOWN) ? 0 : (msg == WM_RBUTTONDOWN) ? 1 : 2;
				MouseButtonPressedEvent e(button);
				data->EventCallback(e);
				return 0;
			}
			case WM_LBUTTONUP:
			case WM_RBUTTONUP:
			case WM_MBUTTONUP:
			{
				int button = (msg == WM_LBUTTONUP) ? 0 : (msg == WM_RBUTTONUP) ? 1 : 2;
				MouseButtonReleasedEvent e(button);
				data->EventCallback(e);
				return 0;
			}
			case WM_KEYDOWN:
			case WM_SYSKEYDOWN:
			{
				int keycode = (int)wParam;
				// lParam bit 30 = previous key state: 1 means this is an
				// auto-repeat (key spam) rather than the first press.
				int repeatCount = (int)((lParam >> 30) & 1);
				KeyPressedEvent e(keycode, repeatCount);
				data->EventCallback(e);
				return 0;
			}
			case WM_KEYUP:
			case WM_SYSKEYUP:
			{
				int keycode = (int)wParam;
				KeyReleasedEvent e(keycode);
				data->EventCallback(e);
				return 0;
			}
		}

		return DefWindowProcA(hWnd, msg, wParam, lParam);
	}
}
