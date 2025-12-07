#pragma once

#include <windows.h>

class Graphics
{
public:
	Graphics(HWND hwnd, int screenWidth, int screenHeight);
	~Graphics();
	void Render();

private:
	HWND m_hwnd;
	int m_screenWidth;
	int m_screenHeight;

	bool m_vsyncEnabled = true;
	float m_sreenNear = 1.0f;
	float m_sreenFar = 1000.0f;
};

