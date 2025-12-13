#pragma once

#include <windows.h>
#include <memory>

#include "application.h"
#include "graphics.h"
#include "input.h"

class System
{
public:
	System(LPCWSTR appname, int screenWidth, int screenHeight);
	~System();

	System(const System& other) = delete;
	System& operator=(const System& other) = delete;

	System(System&& other) = delete;
	System& operator=(System&& other) = delete;

	void Run();

public:
	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	void Initialize();
	void InitializeWindow();
	void Finalize();
	bool Frame();

private:
	HWND m_hwnd;
	HINSTANCE m_hinstance;
	LPCWSTR m_appname;

	int m_screenWidth, m_screenHeight;

	std::unique_ptr<Graphics> m_graphics;
	std::unique_ptr<Input> m_input;
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static System* SystemInstance = nullptr;
