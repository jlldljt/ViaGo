#include "MY.h"
#ifdef FRAME_1
CMyWinApp theApp;//由于是全局对象，所以构造在main函数之前

//-------------------------------
//main
//-------------------------------
void main()
{
	CWinApp* pApp = AfxGetApp();
	getchar();
}
#endif

#ifdef FRAME_2
CMyWinApp theApp;//由于是全局对象，所以构造在main函数之前

BOOL CMyWinApp::InitInstance() {
	cout << "CMyWinApp::InitInstance \n";
	m_pMainWnd = new CMyFrameWnd;
	return TRUE;
}

CMyFrameWnd::CMyFrameWnd() {
	cout << "CMyFrameWnd::CMyFrameWnd \n";
	Create();
}

//-------------------------------
//main
//-------------------------------
void main()
{
	CWinApp* pApp = AfxGetApp();

	pApp->InitApplication();
	pApp->InitInstance();
	pApp->Run();
	getchar();
}
#endif