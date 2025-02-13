
#include "ChaosX.h"
class Sandbox :public  ChaosX::Application
{
public:
	Sandbox()
	{

	}

	~Sandbox()
	{

	}

};

ChaosX::Application* ChaosX::CreateApplication()
{
	return  new			Sandbox();
}
