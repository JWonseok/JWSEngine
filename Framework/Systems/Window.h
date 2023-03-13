#pragma once

class Window
{
public:
	static WPARAM Run(class IExecute* main);

private:
	static void Create();
	static void Destroy();

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

	static void MainRender();

private:
	static class IExecute* mainExecute;
};

