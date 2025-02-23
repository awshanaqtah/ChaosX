#pragma once
#include "cspch.h"
#include "Core.h"
#include "Event/Event.h"
namespace ChaosX
{


	class ChaosX_API Application
	{
	public:


		Application();

		virtual ~Application();



		void Run();


	};
	Application* CreateApplication();

}
