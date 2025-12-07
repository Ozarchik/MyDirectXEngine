#pragma once

#include <windows.h>
#include <memory>
#include "d3dclass.h"

class Graphics
{
public:
	Graphics(HWND hwnd, int screenWidth, int screenHeight);
	~Graphics();
	void Render();

private:
	HWND m_hwnd;
	std::unique_ptr<D3DClass> m_directx;

	int m_screenWidth;
	int m_screenHeight;

	bool m_vsync = true;
	bool m_fullscreen = false;
	float m_screenNear = 1.0f;
	float m_screenFar = 1000.0f;
};

