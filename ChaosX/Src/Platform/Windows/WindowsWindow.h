#pragma once
#include "ChaosX/Window.h"
#include <Windows.h>

namespace ChaosX
{
	// Win32 implementation of the Window interface.
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		unsigned int GetWidth() const override { return m_Data.Width; }
		unsigned int GetHeight() const override { return m_Data.Height; }

		void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }

	private:
		void Init(const WindowProps& props);
		void Shutdown();

		static LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

		HWND m_Hwnd = nullptr;

		// Stored behind GWLP_USERDATA so the static WindowProc can reach the
		// per-window state (and the event callback) for this HWND.
		struct WindowData
		{
			std::string Title;
			unsigned int Width = 0;
			unsigned int Height = 0;
			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};
}
