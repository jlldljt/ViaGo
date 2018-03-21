#define FRAME_2
#ifdef FRAME_1
#include <iostream>
using namespace std;

class CObject
{
public:
	CObject::CObject() { cout << "CObject Constructor \n"; }
	CObject::~CObject() { cout << "CObject Destructor \n"; getchar(); }
};

class CCmdTarget : public CObject
{
public:
	CCmdTarget::CCmdTarget() { cout << "CCmdTarget Constructor \n"; }
	CCmdTarget::~CCmdTarget() { cout << "CCmdTarget Destructor \n"; }
};
/*CWinThread*/
class CWinThread : public CCmdTarget
{
public:
	CWinThread::CWinThread() { cout << "CWinThread Constructor \n"; }
	CWinThread::~CWinThread() { cout << "CWinThread Destructor \n"; }
};

class CWinApp : public CWinThread
{
public:
	CWinApp* m_pCurrentWinApp;

public:
	CWinApp::CWinApp() {
		m_pCurrentWinApp = this;
		cout << "CWinApp Constructor \n";
	}
	CWinApp::~CWinApp() { cout << "CWinApp Destructor \n"; }
};
/*CDocument*/
class CDocument : public CCmdTarget
{
public:
	CDocument::CDocument() { cout << "CDocument Constructor \n"; }
	CDocument::~CDocument() { cout << "CDocument Destructor \n"; }
};
/*CWnd*/
class CWnd : public CCmdTarget
{
public:
	CWnd::CWnd() { cout << "CWnd Constructor \n"; }
	CWnd::~CWnd() { cout << "CWnd Destructor \n"; }
};

class CFrameWnd : public CWnd
{
public:
	CFrameWnd::CFrameWnd() { cout << "CFrameWnd Constructor \n"; }
	CFrameWnd::~CFrameWnd() { cout << "CFrameWnd Destructor \n"; }
};

class CView : public CWnd
{
public:
	CView::CView() { cout << "CView Constructor \n"; }
	CView::~CView() { cout << "CView Destructor \n"; }
};

/*global function*/
CWinApp* AfxGetApp();
#endif
#ifdef FRAME_2
/*********Frame2 start*********/
#define BOOL int
#define TRUE 1
#define FALSE 0
/*********Frame2 end*********/
#include <iostream>
using namespace std;

class CObject
{
public:
	CObject::CObject() {  }
	CObject::~CObject() {  }
};

class CCmdTarget : public CObject
{
public:
	CCmdTarget::CCmdTarget() {  }
	CCmdTarget::~CCmdTarget() {  }
};
/*CWinThread*/
class CWinThread : public CCmdTarget
{
public:
	CWinThread::CWinThread() {  }
	CWinThread::~CWinThread() {  }

	virtual BOOL InitInstance() {
		cout << "CWinThread::InitInstance \n";
		return TRUE;
	}

	virtual int Run() {
		cout << "CWinThread::Run \n";
		return 1;
	}
};

class CWnd;

class CWinApp : public CWinThread
{
public:
	CWinApp* m_pCurrentWinApp;
	CWnd *m_pMainWnd;
public:
	CWinApp::CWinApp() { m_pCurrentWinApp = this; }
	CWinApp::~CWinApp() {  }

	virtual BOOL InitApplication() {
		cout << "CWinApp::InitApplication \n";
		return TRUE;
	}
	virtual BOOL InitInstance() {
		cout << "CWinApp::InitInstance \n";
		return TRUE;
	}
	virtual int Run() {
		cout << "CWinApp::Run \n";
		return CWinThread::Run();
	}
};
/*CDocument*/
class CDocument : public CCmdTarget
{
public:
	CDocument::CDocument() {  }
	CDocument::~CDocument() {  }
};
/*CWnd*/
class CWnd : public CCmdTarget
{
public:
	CWnd::CWnd() {  }
	CWnd::~CWnd() {  }

	virtual BOOL Create();
	BOOL CreateEx();
	virtual BOOL PreCreateWindow();
};

class CFrameWnd : public CWnd
{
public:
	CFrameWnd::CFrameWnd() {  }
	CFrameWnd::~CFrameWnd() {  }
	
	BOOL Create();
	virtual BOOL PreCreateWindow();
};

class CView : public CWnd
{
public:
	CView::CView() {  }
	CView::~CView() {  }
};

/*global function*/
CWinApp* AfxGetApp();

#endif


