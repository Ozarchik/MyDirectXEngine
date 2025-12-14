#pragma once

#include <memory>
#include "d3dclass.h"
#include "camera.h"
#include "model.h"
#include "colorshader.h"

#define FULLSCREEN false
#define VSYNC true
#define SCREEN_NEAR 1
#define SCREEN_FAR  1000

class Application
{
public:
	Application(HWND hwnd, int screenWidth, int screenHeight);

	void Render();

private:
	std::unique_ptr<ColorShader> m_shader;
	std::unique_ptr<Model> m_model;
	std::unique_ptr<Camera> m_camera;
	std::unique_ptr<D3DClass> m_directx;
};

