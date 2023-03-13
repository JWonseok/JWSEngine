#include "Framework.h"
#include "Window.h"
#include "IExecute.h"

IExecute* Window::mainExecute = NULL;

WPARAM Window::Run(IExecute* main)
{
	mainExecute = main;
	Create();

	D3DDesc desc = D3D::GetDesc();

	D3D::Create();
	Keyboard::Create();

	mainExecute->Initialize();

	MSG msg = { 0 };
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			MainRender();
		}
	}
	mainExecute->Destroy();

	Keyboard::Delete();
	D3D::Delete();

	Destroy();

	return msg.wParam;
}

void Window::Create()
{
	D3DDesc desc = D3D::GetDesc();

	WNDCLASSEX wndClass;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wndClass.hIconSm = wndClass.hIcon;
	wndClass.hInstance = desc.Instance;
	wndClass.lpfnWndProc = (WNDPROC)WndProc;
	wndClass.lpszClassName = desc.AppName.c_str();
	wndClass.lpszMenuName = NULL;
	wndClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wndClass.cbSize = sizeof(WNDCLASSEX);

	WORD wHr = RegisterClassEx(&wndClass);
	assert(wHr != 0);

	if (desc.bFullScreen)
	{
		DEVMODE devMode = { 0 };
		devMode.dmSize = sizeof(DEVMODE);
		devMode.dmPelsWidth = (DWORD)desc.Width;
		devMode.dmPelsHeight = (DWORD)desc.Height;
		devMode.dmBitsPerPel = 32;
		devMode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&devMode, CDS_FULLSCREEN);
	}

	desc.Handle = CreateWindowEx(WS_EX_APPWINDOW, desc.AppName.c_str(), desc.AppName.c_str(),
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, desc.Instance, NULL);
	assert(desc.Handle != NULL);
	D3D::SetDesc(desc);

	RECT rt = { 0, 0, (LONG)desc.Width, (LONG)desc.Height };

	UINT centerX = (GetSystemMetrics(SM_CXSCREEN) - (UINT)desc.Width) / 2;
	UINT centerY = (GetSystemMetrics(SM_CYSCREEN) - (UINT)desc.Height) / 2;

	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, FALSE);
	MoveWindow(desc.Handle, centerX, centerY, rt.right - rt.left, rt.bottom - rt.top, true);
	ShowWindow(desc.Handle, SW_SHOWNORMAL);
	SetForegroundWindow(desc.Handle);
	SetFocus(desc.Handle);

	ShowCursor(true);
}

void Window::Destroy()
{
	D3DDesc desc = D3D::GetDesc();

	if (desc.bFullScreen)
		ChangeDisplaySettings(NULL, 0);

	DestroyWindow(desc.Handle);

	UnregisterClass(desc.AppName.c_str(), desc.Instance);
}

LRESULT Window::WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	if (iMessage == WM_SIZE)
	{
		if (mainExecute != NULL)
		{
			float width = (float)LOWORD(lParam);
			float height = (float)HIWORD(lParam);

			if (D3D::Get() != NULL)
				D3D::Get()->ResizeScreen(width, height);

			mainExecute->ResizeScreen();
		}
	}

	if (iMessage == WM_CLOSE || iMessage == WM_DESTROY)
	{
		PostQuitMessage(0);

		return 0;
	}

	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}

void Window::MainRender()
{
	mainExecute->Update();
	mainExecute->PreRender();

	D3DDesc desc = D3D::GetDesc();

	D3D::Get()->SetRenderTarget();
	D3D::Get()->Clear(desc.Background);
	{
		mainExecute->Render();
		mainExecute->PostRender();
	}
	D3D::Get()->Present();
}
