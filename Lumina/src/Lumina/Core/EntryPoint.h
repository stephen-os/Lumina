#pragma once

#include "Application.h"

#ifdef WL_PLATFORM_WINDOWS

extern Lumina::Application* Lumina::CreateApplication(int argc, char** argv);

namespace Lumina 
{
	int Main(int argc, char** argv)
	{
		Lumina::Application* app = Lumina::CreateApplication(argc, argv);
		app->Run();
		delete app;

		return 0;
	}
}

#ifdef WL_DIST

#include <Windows.h>

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
{
	return Lumina::Main(__argc, __argv);
}

#else

int main(int argc, char** argv)
{
	return Lumina::Main(argc, argv); 
}

#endif // WL_DIST

#endif // WL_PLATFORM_WINDOWS
