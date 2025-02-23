#include "cspch.h"
#include "Application.h"
#include "Cspch.h"
#include "ChaosX/Event/ApplicationEvent.h"
#include "ChaosX/Log.h"


namespace ChaosX
{

	Application::Application()
	{
	}
	Application::~Application()
	{
	}
	void Application::Run()
	{
		WindowResizeEvent e(1920, 1080);
		SPDLOG_TRACE("{}", e.ToString());  // Correct variable name
		CS_TRACE(e.ToString());            // Explicitly call ToString()
		while (true);
	}

}