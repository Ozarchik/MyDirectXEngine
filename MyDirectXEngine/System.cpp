#include "System.h"

System::System(LPCWSTR appname, int screenWidth, int screenHeight)
	: m_appname(appname)
	, m_screenWidth(screenWidth)
	, m_screenHeight(screenHeight)
{
	Initialize();
}

System::~System()
{
	Finalize();
}

void System::Initialize()
{
	SystemInstance = this;
	m_hinstance = GetModuleHandle(NULL);

	InitializeWindow();

	m_input = std::make_unique<Input>();
	m_graphics = std::make_unique<Graphics>(m_hwnd, m_screenWidth, m_screenHeight);
}

void System::InitializeWindow()
{
	WNDCLASSEX wc = { 0 };
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_INFORMATION);
	wc.hIconSm = LoadIcon(NULL, IDI_QUESTION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszClassName = m_appname;
	wc.cbSize = sizeof(WNDCLASSEX);

	RegisterClassEx(&wc);

	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	if (m_screenWidth > screenWidth)
		m_screenWidth = screenWidth;
	if (m_screenHeight > screenHeight)
		m_screenHeight = screenHeight;

	DEVMODE devScreenSettings = { 0 };
	int posX = 0;
	int posY = 0;

	if (FULLSCREEN) {
		devScreenSettings.dmSize = sizeof(devScreenSettings);
		devScreenSettings.dmPelsWidth = (unsigned long)m_screenWidth;
		devScreenSettings.dmPelsHeight = (unsigned long)m_screenHeight;
		devScreenSettings.dmBitsPerPel = 32;
		devScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&devScreenSettings, CDS_FULLSCREEN);
	}
	else {
		posX = (GetSystemMetrics(SM_CXSCREEN) - m_screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - m_screenHeight) / 2;
	}

	m_hwnd = CreateWindowEx(
		WS_EX_APPWINDOW,
		m_appname,
		m_appname,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX,
		posY,
		m_screenWidth,
		m_screenHeight,
		NULL,
		NULL,
		m_hinstance,
		NULL
	);

	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);
	ShowCursor(false);
}

void System::Finalize()
{
	DestroyWindow(m_hwnd);
	UnregisterClass(m_appname, m_hinstance);
}

void System::Run()
{
	MSG msg = { 0 };
	bool done = false;
	
	while (!done) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		
		if (msg.message == WM_QUIT) {
			done = true;
		}

		if (m_input->IsKeyUp(VK_ESCAPE)) {
			done = true;
		}

		m_graphics->Render();
	}
}

LRESULT System::MessageHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg) {
		case WM_KEYUP: 
			m_input->KeyUp((unsigned int)wparam);
			return 0;
		case WM_KEYDOWN:
			m_input->KeyDown((unsigned int)wparam);
			return 0;
		default:
			return DefWindowProc(hwnd, msg, wparam, lparam);
	}

	return 0;
}

bool System::Frame()
{
	return false;
}

LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg) {
		case WM_QUIT:
			PostQuitMessage(0);
			return 0;
		case WM_CLOSE:
			PostQuitMessage(0);
			return 0;
		default:
			return SystemInstance->MessageHandler(hwnd, msg, wparam, lparam);
	}

	return 0;
}
