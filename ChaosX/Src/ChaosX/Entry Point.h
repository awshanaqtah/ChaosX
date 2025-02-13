#pragma once

#include "Application.h"


#ifdef CS_PLATFORM_WINDOWS

extern ChaosX::Application* ChaosX::CreateApplication();

int main(int agrc, char** argv)
{

    auto app = ChaosX::CreateApplication();
    app->Run();
    delete app;

}

#endif // ChaosX PLat_from Windows