/*
*演示win32窗体的建立,演示课本，采用了多字节环境
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
	if (!hPrevInstance)//在win3.x时代，用来判断是否是第一个实例，现已无用
		if (!InitApplication(hInstance))
			return FALSE;
	if (!InitInstance(hInstance, nCmdShow))
		return FALSE;

while (GetMessage(&msg, NULL, 0, 0)) {
	TranslateMessage(&msg);//转换键盘消息
	DispatchMessage(&msg);//分派消息，通过user模块的协助，送达串口函数wndproc
}
return (msg.wParam);  // 传回postquitmessage的参数
}

//-----------------------------
//InitApplication - a class about register window
//-----------------------------
BOOL InitApplication(HINSTANCE hInstance) {
	WNDCLASS wc = { 0 };
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = NULL;// LoadIcon(hInstance, "basicicon");
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = "TestMenu";
	wc.lpszClassName = _szAppName;
	return (RegisterClass(&wc));
}
//-----------------------------
//InitInstance - create window
//-----------------------------
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
	_hInst = hInstance;

	_hWnd = CreateWindow(
		_szAppName,
		_szTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL
		);
	if (!_hWnd)
		return (FALSE);

	ShowWindow(_hWnd, nCmdShow);
	UpdateWindow(_hWnd);
	return (TRUE);
}
//-----------------------------
//InitInstance - create window
//-----------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	int wmId, wmEvent;

	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(_hInst,
				"AboutBox",
				hWnd,
				(DLGPROC)About
				);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;

		default:
			return (DefWindowProc(hWnd, message, wParam, lParam));//Windows默认消息处理函数
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return (DefWindowProc(hWnd, message, wParam, lParam));//break;
	}
	return(0);
}
//-----------------------------
//About - dialog
//-----------------------------
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
	UNREFERENCED_PARAMETER(lParam);

	switch (message)
	{
	case WM_INITDIALOG:
		return (TRUE);

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK
			|| LOWORD(wParam) == IDCANCEL) {
			EndDialog(hDlg, TRUE);
			return (TRUE);
		}
		break;

	default:
		break;
	}
	return (FALSE);
}