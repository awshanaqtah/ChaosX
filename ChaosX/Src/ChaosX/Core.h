#pragma once

#ifdef CS_PLATFORM_WINDOWS
	#ifdef CS_BUILD_DLL
		#define ChaosX_API __declspec(dllexport)
	#else
		#define ChaosX_API __declspec(dllimport)
	#endif
#else
	#error ChaosX only supports Windows!
#endif