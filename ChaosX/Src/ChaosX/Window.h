#pragma once
#include "cspch.h"
#include "ChaosX/Core.h"
#include "ChaosX/Event/Event.h"

namespace ChaosX
{
	// Settings used when creating a window.
	struct WindowProps
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProps(const std::string& title = "ChaosX Engine",
					unsigned int width = 1280,
					unsigned int height = 720)
			: Title(title), Width(width), Height(height) {}
	};

	// Platform-agnostic interface for a desktop window.
	// The concrete implementation is chosen by Window::Create().
	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() = default;

		// Pump the OS message queue once per frame.
		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		// The window forwards every event it produces to this callback.
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;

		static Window* Create(const WindowProps& props = WindowProps());
	};
}
