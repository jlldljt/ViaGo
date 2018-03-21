/*
*演示win32窗体的建立
*/

#include <Windows.h>
#include "resource.h"
#include "test.h"

HINSTANCE _hInst;
HWND      _hWnd;

char _szAppName[] = "test";
char _szTitle[]   = "test app";
//-----------------------------
//WinMain - where does the program start
//-----------------------------
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
	LPSTR lpCmdLine, int nCmdShow) {
	MSG msg;
	UNREFERENCED_PARAMETER(lpCmdLine);
	if (!hPrevInstance)
		if (!InitApplication(hInstance))
			return FALSE;
	if (!InitInstance(hInstance, nCmdShow))
		return FALSE;

	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (msg.wParam);  // 传回postquitmessage的参数
}

//-----------------------------
//InitApplication - a class about register window
//-----------------------------
BOOL InitApplication(HANDLE hand) {
	return 1;
}
BOOL InitInstance(HANDLE hand, int inta) {
	return 1;
}


//LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
//LRESULT CALLBACK About(HWND, UINT, WPARAM, LPARAM);