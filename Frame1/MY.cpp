#include "MY.h"
#ifdef FRAME_1
CMyWinApp theApp;//������ȫ�ֶ������Թ�����main����֮ǰ

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
CMyWinApp theApp;//������ȫ�ֶ������Թ�����main����֮ǰ

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