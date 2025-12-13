#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "system.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hprevInstance, PSTR pScmdLine, int iCmdShow)
{
	System system(L"Hello Window", 1200, 800);

	system.Run();

	return 0;
}