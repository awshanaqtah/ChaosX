#pragma once
#include "cspch.h"
#include "Core.h"
#include "Event/Event.h"
#include "ChaosX/Event/ApplicationEvent.h"
#include "ChaosX/Window.h"

namespace ChaosX
{
	// m_Window is a std::unique_ptr (no dll-interface) but it is purely
	// internal state — clients only ever call the exported member functions,
	// never touch the member directly — so C4251 is safe to silence here.
#pragma warning(push)
#pragma warning(disable: 4251)
	class ChaosX_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
	};
#pragma warning(pop)

	// Defined by the client (e.g. Sandbox).
	Application* CreateApplication();
}
