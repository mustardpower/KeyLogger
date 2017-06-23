#pragma once
#include <Windows.h>

#define KEY_LOGGER_API   __declspec( dllexport )

extern "C"
{
	KEY_LOGGER_API LRESULT CALLBACK KeyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam);
}
