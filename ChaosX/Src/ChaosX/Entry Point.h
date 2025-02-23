#pragma once
#include "cspch.h"

extern ChaosX::Application* ChaosX::CreateApplication();

int main(int agrc, char** argv)
{
    ChaosX::Log::Init();
    CS_CORE_WARN("Initialized Log");
    int a = 10;
    CS_INFO("Hello! Var={0}" ,a);
    

    auto app = ChaosX::CreateApplication();
    app->Run();
    delete app;

}