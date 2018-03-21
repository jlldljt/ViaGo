

#include "MY.h"//因为包含了MY.h所以这里不是很愉快
#ifdef FRAME_1
extern CMyWinApp theApp;

CWinApp* AfxGetApp()
{
	return theApp.m_pCurrentWinApp;
}
#endif

#ifdef FRAME_2
extern CMyWinApp theApp;

BOOL CWnd::Create() {
	cout << "CWnd::Create \n";
	return TRUE;
}

BOOL CWnd::CreateEx() {
	cout << "CWnd::CreateEx \n";
	PreCreateWindow();
	return TRUE;
}

BOOL CWnd::PreCreateWindow() {
	cout << " CWnd::PreCreateWindow \n";
	return TRUE;
}

BOOL CFrameWnd::Create() {
	cout << "CFrameWnd::Create \n";
	CreateEx();
	return TRUE;
}

BOOL CFrameWnd::PreCreateWindow() {
	cout << "CFrameWnd::PreCreateWindow \n";
	return TRUE;
}

CWinApp* AfxGetApp()
{
	return theApp.m_pCurrentWinApp;
}

#endif