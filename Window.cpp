#include <windows.h>
#include <stdlib.h>
#include <cstdio>
#include <fstream>
#include <string>
#include <io.h>
#include <algorithm>


HMENU hMenu;
HWND hMainWindow;
HWND hName;
HWND hAge;
HWND hResult;
HWND hEdit;


void AddMenus(HWND hWnd)
{
	hMenu = CreateMenu();
	HMENU hFileMenu = CreateMenu();
	HMENU hSubMenu = CreateMenu();

	AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, L"File");
		AppendMenu(hFileMenu, MF_STRING, 1, L"New");
		AppendMenu(hFileMenu, MF_POPUP, (UINT_PTR)hSubMenu, L"Open");
			AppendMenu(hSubMenu, MF_STRING, 4, L"Sub Open");
		AppendMenu(hFileMenu, MF_STRING, 5, L"Save");
		AppendMenu(hFileMenu, MF_SEPARATOR, NULL, NULL);
		AppendMenu(hFileMenu, MF_STRING, 3, L"Exit");
	AppendMenu(hMenu, MF_STRING, NULL, L"Help");

	SetMenu(hWnd, hMenu);
}

void AddControls(HWND hWnd)
{
	CreateWindowW(L"Static", L"Name :", WS_VISIBLE | WS_CHILD, 100, 10, 98, 38, hWnd, NULL, NULL, NULL);
	hName = CreateWindowW(L"Edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, 200, 10, 98, 38, hWnd, NULL, NULL, NULL);
	CreateWindowW(L"Static", L"Age :", WS_VISIBLE | WS_CHILD, 100, 50, 98, 38, hWnd, NULL, NULL, NULL);
	hAge = CreateWindowW(L"Edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, 200, 50, 98, 38, hWnd, NULL, NULL, NULL);
	hResult = CreateWindowW(L"Static", L"", WS_VISIBLE | WS_CHILD, 100, 150, 300, 38, hWnd, NULL, NULL, NULL);
	hEdit = CreateWindowW(L"Edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | WS_VSCROLL, 30, 200, 420, 230, hWnd, NULL, NULL, NULL);

	CreateWindowW(L"Button", L"Generate", WS_VISIBLE | WS_CHILD | WS_BORDER, 150, 100, 98, 38, hWnd, (HMENU)2, NULL, NULL);
}

void WriteFile(wchar_t* SavePath)
{
	/*FILE** File{};
	_wfopen_s(File, SavePath, L"w");

	int TextSize = GetWindowTextLength(hEdit);
	wchar_t* wsLine = new wchar_t[TextSize];
	GetWindowText(hEdit, wsLine, TextSize);

	fwrite(wsLine, TextSize, 1, *File);
	fclose(*File);*/

	int TextSize;
	std::wstring wsLine;

	std::wofstream File(SavePath);

	TextSize = GetWindowTextLength(hEdit);
	wchar_t* wcLine = new wchar_t[TextSize + 1];

	GetWindowText(hEdit, wcLine, TextSize + 1);

	wsLine = std::wstring(wcLine);

	wsLine.erase(remove(wsLine.begin(), wsLine.end(), '\n'), wsLine.end());

	File << wsLine;

	delete[] wcLine;
	File.close();
}

void SaveFile(HWND hWnd)
{
	OPENFILENAME ofn;

	wchar_t FileName[100]{};

	ZeroMemory(&ofn, sizeof(OPENFILENAME));

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFile = FileName;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = 100;
	ofn.lpstrFilter = L"Text Files(*.txt)\0*.txt\0";
	ofn.nFilterIndex = 1;

	GetSaveFileName(&ofn);

	WriteFile(ofn.lpstrFile);
}

void DisplayFile(wchar_t* OpenPath)
{
	/*FILE* File;
	File = _wfopen_s(File, path, L"rb");
	fseek(File, 0, SEEK_END);
	int Size = ftell(File);
	rewind(File);
	wchar_t* Data = new wchar_t(Size + 1);
	fread(Data, Size, 1, File);
	Data[Size] = '\0';*/

	std::wstring wsLine;
	std::wstring wsLine2;
	LPCWSTR lLine;

	std::wifstream File(OpenPath);

	
	
	while (getline(File, wsLine))
	{
		//wsLine.erase(remove(wsLine.begin(), wsLine.end(), '\r\n'), wsLine.end());

		while (true)
		{
			if (wsLine.find(L'\r') == std::string::npos)
			{
				break;
			}

			wsLine.replace(wsLine.find(L'\r'), 1, L"µ");
		}

		while (true)
		{
			if (wsLine.find(L'µ') == std::string::npos)
			{
				break;
			}

			wsLine.replace(wsLine.find(L'µ'), 1, L"\r\n");
		}

		wsLine2 += wsLine += L"\r\n";
	}
	

	/*while (getline(File, wsLine))
	{
		//wsLine.erase(remove(wsLine.begin(), wsLine.end(), '\r\n'), wsLine.end());
		wsLine.replace(wsLine.find(L'\r'), 1, L"\r\n");
		wsLine2 += wsLine += L"\r\n";
	}*/
	
	lLine = wsLine2.c_str();

	SetWindowText(hEdit, lLine);

	File.close();
}

void OpenFile(HWND hWnd)
{
	OPENFILENAME ofn;

	wchar_t FileName[100]{};

	ZeroMemory(&ofn, sizeof(OPENFILENAME));

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFile = FileName;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = 100;
	ofn.lpstrFilter = L"Text Files(*.txt)\0*.txt\0";
	ofn.nFilterIndex = 1;

	GetOpenFileName(&ofn);

	DisplayFile(ofn.lpstrFile);
}

void displayDialog(HWND hWnd)
{
	HWND hDlg;

	hDlg = CreateWindowW(L"myDialogClass", L"Dialog", WS_VISIBLE | WS_OVERLAPPEDWINDOW, 350, 400, 250, 200, hWnd, NULL, NULL, NULL);
	
	CreateWindowW(L"Button", L"Close", WS_VISIBLE | WS_CHILD, 20, 20, 100, 40, hDlg, (HMENU)1, NULL, NULL);

	EnableWindow(hWnd, false);
}

LRESULT CALLBACK DialogProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_COMMAND:
		switch (wp)
		{
		case 1:
			EnableWindow(hMainWindow, true);
			DestroyWindow(hWnd);
			break;
		}
		break;
	case WM_CLOSE:
		EnableWindow(hMainWindow, true);
		DestroyWindow(hWnd);
		break;
	default:
		return DefWindowProcW(hWnd, msg, wp, lp);
	}
}

void registerDialogClass(HINSTANCE hDcInst)
{
	WNDCLASSW dc = { 0 };
	dc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	dc.hCursor = LoadCursor(NULL, IDC_ARROW);
	dc.hInstance = hDcInst;
	dc.lpszClassName = L"myDialogClass";
	dc.lpfnWndProc = DialogProcedure;

	RegisterClassW(&dc);
}

LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	int Val;

	switch (msg)
	{
	case WM_COMMAND:
		switch (wp)
		{
		case 1:
			displayDialog(hWnd);
			break;

		case 2:
			wchar_t Name[30];
			wchar_t Age[10];
			wchar_t Result[40];

			GetWindowText(hName, Name, 30);
			GetWindowText(hAge, Age, 10);
			GetWindowText(hResult, Result, 40);

			wcscpy_s(Result, L"Name is ");
			wcscat_s(Result, Name);
			wcscat_s(Result, L". Age: ");
			wcscat_s(Result, Age);

			if (wcscmp(Name, L"") == 0 || wcscmp(Age, L"") == 0)
			{
				MessageBoxW(hWnd, L"You cannot generate text without name or age", L"Error", MB_OK | MB_ICONERROR);
				break;
			}

			SetWindowText(hResult, Result);
			break;

		case 3:
			Val = MessageBoxW(hWnd, L"Do you really want to exit the program?", L"Exit", MB_YESNO | MB_ICONQUESTION);

			if (Val == IDYES)
			{
				DestroyWindow(hWnd);
			}
			break;
		case 4:
			OpenFile(hWnd);
			break;
		case 5:
			SaveFile(hWnd);
			break;
		}
		break;
	case WM_CREATE:
		AddMenus(hWnd);
		AddControls(hWnd);
		break;
		
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProcW(hWnd, msg, wp, lp);
	}
}


int WINAPI WinMain(_In_ HINSTANCE hInst, _In_opt_ HINSTANCE hPrevInst, _In_ LPSTR args, _In_ int nShowCmd)
{
	std::locale::global(std::locale("Russian_Russia"));

	WNDCLASSW wc = { 0 };
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hInstance = hInst;
	wc.lpszClassName = L"myWindowClass";
	wc.lpfnWndProc = WindowProcedure;

	if (!RegisterClassW(&wc))
	{
		MessageBox(NULL, L"Failed to register window class", L"Error", MB_OK);
		return 1;
	}

	registerDialogClass(hInst);

	hMainWindow = CreateWindowW(L"MyWindowClass", L"Window", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 500, 500, NULL, NULL, NULL, NULL);

	MSG msg = { 0 };
	while (GetMessage(&msg, NULL, NULL, NULL))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}

//yaaaay :D