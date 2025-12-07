#pragma once

#include <windows.h>

class Input
{
public:
	Input();

	void KeyUp(unsigned int key);
	void KeyDown(unsigned int key);
	bool IsKeyUp(unsigned int key);
	bool IsKeyDown(unsigned int key);

private:
	bool m_keys[256];
};

