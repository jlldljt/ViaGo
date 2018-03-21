#include <iostream>
#include "MFC.h"

using namespace std;
#ifdef FRAME_1
class CMyWinApp : public CWinApp
{
public:
	CMyWinApp::CMyWinApp() { cout << "CMyWinApp Constructor \n"; }
	CMyWinApp::~CMyWinApp() { cout << "CMyWinApp Destructor \n"; }
};

class CMyFrameWnd : public CFrameWnd
{
public:
	CMyFrameWnd() { cout << "CMyFrameWnd Constructor \n"; }
	~CMyFrameWnd() { cout << "CMyFrameWnd Destructor \n"; }
};
#endif

#ifdef FRAME_2
class CMyWinApp : public CWinApp
{
public:
	CMyWinApp::CMyWinApp() {  }
	CMyWinApp::~CMyWinApp() {  }
	virtual BOOL InitInstance();
};

class CMyFrameWnd : public CFrameWnd
{
public:
	CMyFrameWnd();
	~CMyFrameWnd() {  }
};
#endif