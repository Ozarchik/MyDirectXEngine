#pragma once

#include <windows.h>
#include <memory>
#include "d3dclass.h"
#include "model.h"
#include "colorshader.h"
#include "camera.h"

#define FULLSCREEN false
#define VSYNC true
#define SCREEN_NEAR 0.1f
#define SCREEN_FAR  1000.0f

class Graphics
{
public:
	Graphics(HWND hwnd, int screenWidth, int screenHeight);
	~Graphics();
	void Render();
	void setFullscreen(bool state);

private:
	HWND m_hwnd;
	std::unique_ptr<D3DClass> m_directx;
	std::unique_ptr<ColorShader> m_shader;
	std::unique_ptr<Model> m_model;
	std::unique_ptr<Camera> m_camera;

	int m_screenWidth;
	int m_screenHeight;

	bool m_vsync = true;
	bool m_fullscreen = false;
	float m_screenNear = 1.0f;
	float m_screenFar = 1000.0f;
};

