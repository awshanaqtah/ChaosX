#pragma once
#include "Core.h"
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
