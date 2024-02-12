/*
	PrankProgram.Win32_x64.RICK-ROLL
	Customizable rick roll
	Copyright (C) 2024 AleX-1337

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program. If not, see <https://www.gnu.org/licenses/>.

	***

	THE CREATOR OF THIS PROGRAM (AleX-1337) IS NOT RESPONIBLE FOR DAMAGES
	THAT THIS PROGRAM MAY CAUSE! EVEN IF THIS IS NOT MEANT TO BE DANGEROUS, IT
	MAY SITLL CAUSE DAMAGES. USE IT AT YOUR OWN RISK!
*/

#include <windows.h>
#include <stdio.h>
#include <conio.h>

void Spam();

HCRYPTPROV hProv;
int Random() //Random number generator using windows encryption tools
{
	if (!hProv)
		CryptAcquireContextW(&hProv, 0, 0, PROV_RSA_FULL, CRYPT_SILENT | CRYPT_VERIFYCONTEXT);

	int output;
	CryptGenRandom(hProv, sizeof(output), (BYTE*)(&output));

	return output & 0x7FFFFFFF;
}

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) //Minimize all windows
{
	if (IsWindowVisible(hwnd))
		ShowWindow(hwnd, SW_MINIMIZE);

	return 1;
}

void Roll()
{
	wchar_t wpPath[MAX_PATH];
	GetFullPathNameW(L"rick.png", MAX_PATH, wpPath, 0); //Load wallpaper
	SystemParametersInfoW(SPI_SETDESKWALLPAPER, 0, wpPath, SPIF_SENDCHANGE); //Change wallpaper

	HWND shlw = FindWindowW(L"Shell_TrayWnd", 0);
	SendMessageW(shlw, WM_COMMAND, 419, 0); //Minimize all windows

	while (1)
	{
		mciSendStringW(L"play rick.wav wait", 0, 0, 0); //Play song
		mciSendStringW(L"seek rick.wav to start wait", 0, 0, 0); //Reset song
	}
}

void TimedMode()
{
	int ms = 0;
	printf_s("Please input the amount of milliseconds before rick roll (max 86400000 = 1 day): ");
	scanf_s("%i", &ms); //Read milliseconds
	if (ms > 86400000) ms = 86400000; //Max a day

	ShowWindow(GetConsoleWindow(), SW_HIDE);
	Sleep(ms);

	Roll();
}

void WaitingForInputMode()
{
	printf_s("Do not move mouse or press keys after 10 seconds!\n10");
	Sleep(1000);
	printf_s(", 9");
	Sleep(1000);
	printf_s(", 8");
	Sleep(1000);
	printf_s(", 7");
	Sleep(1000);
	printf_s(", 6");
	Sleep(1000);
	printf_s(", 5");
	Sleep(1000);
	printf_s(", 4");
	Sleep(1000);
	printf_s(", 3");
	Sleep(1000);
	printf_s(", 2");
	Sleep(1000);
	printf_s(", 1");
	Sleep(250);
	printf_s(".");
	Sleep(250);
	printf_s(".");
	Sleep(250);
	printf_s(".");
	Sleep(250);

	ShowWindow(GetConsoleWindow(), SW_HIDE);

	bool input = false;
	while (!input)
	{
		POINT p1;
		GetCursorPos(&p1);

		if (_kbhit())
		{
			input = true;
			_getch();
		}

		if (GetKeyState(VK_LBUTTON) & 0x8000 || GetKeyState(VK_RBUTTON) & 0x8000 || GetKeyState(VK_MBUTTON) & 0x8000)
			input = true;

		POINT p2;
		GetCursorPos(&p2);

		if (p1.x != p2.x || p1.y != p1.y)
			input = true;
	}

	Roll();
}

void Help()
{
	printf_s("Welcome to the RICK-ROLL prank program!\n\nWith this program you can setup rick rolls that start either"
		" after a certain time, or as soon as user moves mouse or presses keys.\nMode 1: rick roll starts after"
		" a specified amount of time in milliseconds (max 86400000 = 1 day).\nMode 2: program waits for 10 seconds."
		" After that, rick roll starts as soon as user moves mouse or presses keys.\n\nPress any key to continue...\n\n"
		"Made with hate by AleX-1337\nhttps://youtube.com/@YT-AleX-1337");

	_getch();

	wchar_t path[MAX_PATH];
	GetModuleFileNameW(0, path, MAX_PATH);
	ShellExecuteW(0, 0, path, 0, 0, SW_SHOWNORMAL);
	ExitProcess(0);
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) //Program entry point
{
	AllocConsole(); //Spawn console

	FILE* stream; //Repen console streams
	freopen_s(&stream, "CONIN$", "r", stdin);
	freopen_s(&stream, "CONOUT$", "w", stdout);
	freopen_s(&stream, "CONOUT$", "w", stderr);

	int mode = 0;
	while (1)
	{
		printf_s("Please select a mode (1 = timed mode; 2 = waiting for input mode; 3 = help) ");
		scanf_s("%i", &mode); //Read mode

		switch (mode)
		{
		case 1:
			TimedMode();
		case 2:
			WaitingForInputMode();
		case 3:
			Help();
		default:
			break;
		}
	}
}