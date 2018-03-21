#include "WindowBsp.h"
#include <assert.h>
//由于是回调，放在类里变成静态，其访问的也必须是静态，而静态的CallBackFunction会出问题。故做成全局
//LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
//CallBackFunction callback = { 0 };

CallBackFunction CWindowBsp::callback_ = { NULL,NULL,NULL };

CWindowBsp::CWindowBsp() //: callback_({NULL,NULL,NULL})
{
  //callback_ = &callback;
  //proc_.in_class_wnd_proc = &CWindowBsp::WndProc;
}


CWindowBsp::~CWindowBsp()
{
}

typedef LRESULT(CALLBACK *WND_PROC)(HWND, UINT, WPARAM, LPARAM);
/*
*	创建w32主窗口
*/
HWND CWindowBsp::CreateWin32Window(
  HINSTANCE hInstance,
  LPCWSTR lpName,
  DWORD dwStyle,
  HWND hWndParent) {

  //【1】窗口创建四步曲之一：开始设计一个完整的窗口类

  WNDCLASSEX wndClass = { 0 };							//用WINDCLASSEX定义了一个窗口类
  wndClass.cbSize = sizeof(WNDCLASSEX);			//设置结构体的字节数大小
  wndClass.style = CS_HREDRAW | CS_VREDRAW;	//设置窗口的样式
  wndClass.lpfnWndProc = WndProc;// (proc_.wnd_proc);					//设置指向窗口过程函数的指针
  wndClass.cbClsExtra = 0;								//窗口类的附加内存，取0就可以了
  wndClass.cbWndExtra = 0;							//窗口的附加内存，依然取0就行了
  wndClass.hInstance = hInstance;						//指定包含窗口过程的程序的实例句柄。
 wndClass.hIcon = (HICON)::LoadImage(NULL, L"5.jpg", IMAGE_BITMAP,0,0,LR_DEFAULTSIZE|LR_LOADFROMFILE);  //本地加载自定义ico图标//LoadIcon(NULL, IDI_APPLICATION);
 
  //wndClass.hIcon = //(HICON)::LoadImage(NULL,L"icon.ico",IMAGE_ICON,0,0,LR_DEFAULTSIZE|LR_LOADFROMFILE);  //本地加载自定义ico图标//LoadIcon(NULL, IDI_APPLICATION);
  wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);    //指定窗口类的光标句柄。
  wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);  //为hbrBackground成员指定一个白色画刷句柄	
  wndClass.lpszMenuName = NULL;						//用一个以空终止的字符串，指定菜单资源的名字。
  wndClass.lpszClassName = lpName;		//用一个以空终止的字符串，指定窗口类的名字。

                      //【2】窗口创建四步曲之二：注册窗口类
  if (!RegisterClassEx(&wndClass))				//设计完窗口后，需要对窗口类进行注册，这样才能创建该类型的窗口
  {
    MessageBox(NULL, TEXT("This program requires Windows NT!"),
      lpName, MB_ICONERROR);
    return NULL;
  }

  //【3】窗口创建四步曲之三：正式创建窗口
  HWND hwnd = CreateWindow(
    lpName,
    NULL,				//窗口的标题，默认没有，可以后面自行设置
    dwStyle,
    0,
    0,
    0,
    0,
    hWndParent,
    NULL,//子窗口，这里要填ID，我这里没填ID貌似也没事，只是不能利用有关ID的一些函数了
    hInstance,
    NULL);

  //【4】窗口创建四步曲之四：窗口的移动、显示与更新
  //MoveWindow(hwnd,cxScreen >>2,cyScreen >>2,cxScreen >>1,	cyScreen >>1, true);		//调整窗口显示时的位置，使窗口左上角位于（250,80）处
  //ShowWindow( hwnd, true );    //调用ShowWindow函数来显示窗口
  //UpdateWindow(hwnd);						//对窗口进行更新，就像我们买了新房子要装修一样

  return hwnd;
}

/************************************************************************/
/*          CACTWindow                                                  */
/************************************************************************/
/*
*	创建主、从窗口
*/
HWND CWindowBsp::CreateBoard(HINSTANCE hInstance,
  LPCWSTR lpName,
  RECT rect,
  HWND parentwnd) {
  //  assert(hwnd_);
  assert(lpName && hInstance);
  assert(rect.bottom > rect.top && rect.right > rect.left);
  //if (NULL == wndProc || NULL == lpName || NULL == hInstance) {
  //	return NULL;
  //}
  DWORD style = !parentwnd ?
    WS_OVERLAPPEDWINDOW | WS_VISIBLE :
    WS_CHILD | WS_VISIBLE | WS_BORDER;

  HWND hwnd = CreateWin32Window(
    hInstance,
    lpName,
    style,
    parentwnd);

  MoveWindow(hwnd, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, true);		//调整窗口显示时的位置，使窗口左上角位于（250,80）处
  ShowWindow(hwnd, true);    //调用ShowWindow函数来显示窗口
  UpdateWindow(hwnd);						//对窗口进行更新，就像我们买了新房子要装修一样

  //m_instance = hInstance;
  //m_name = lpName;
  //hwnd_ = hwnd;
  return hwnd;
}

/*
*	销毁主窗口
*/
bool CWindowBsp::DestoryBoard(
  HINSTANCE hInstance,
  LPCWSTR lpName) {
  //【6】窗口类的注销
  UnregisterClass(lpName, hInstance);  //程序准备结束，注销窗口类
  return 0;
}

//tick：间隔多少ms
void CWindowBsp::Cartoon(DWORD tick)
{
  if (callback_.cartoon)
    callback_.cartoon(tick);
}
///////////////////////////////串口过程，处理窗口消息//////////////////////////////////////////////
LRESULT CALLBACK CWindowBsp::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  static int		cxClient, cyClient, cxBitmap, cyBitmap, cxChild, cyChild;
  //子窗口的窗口句柄

  static HBITMAP	hBitmap;
  HINSTANCE		hInstance;
  HDC				hdc, hdcMem;
  PAINTSTRUCT		ps;
  int				i, j;

  switch (message)						//switch语句开始
  {

  case WM_CREATE:
    /*BITMAP			bitmap;

    hInstance	= ((LPCREATESTRUCT)lParam)->hInstance;
    hBitmap		= LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP1));

    GetObject(hBitmap, sizeof(BITMAP), &bitmap);

    cxBitmap	= bitmap.bmWidth;
    cyBitmap	= bitmap.bmHeight;*/

    hInstance = ((LPCREATESTRUCT)lParam)->hInstance;

    /*for(i=0; i<2; i++)
    hwndChild[i] = CreateWindow(L"childWindow", NULL,
    WS_CHILD | WS_VISIBLE | WS_BORDER,
    0, 0, 0, 0,
    hwnd, NULL, hInstance, NULL);*/
    break;
  case WM_SIZE:
    //确定父窗口客户区大小
    cxClient = LOWORD(lParam);
    cyClient = HIWORD(lParam);

    //确定子窗口大小
    cxChild = cxClient / 4;
    cyChild = cyClient / 4;



    /*        移动子窗口      */
    /*for(i = 0; i < 2 ;i++)
    MoveWindow(hwndChild[i], i * (cxChild + 1),  (cyChild + 1),
    cxChild, cyChild, TRUE);*/
    break;
  case WM_LBUTTONDOWN:
  {
    //  if (callback.lbuttondown)
    //    return callback.lbuttondown( hwnd,  message,  wParam,  lParam);
    //SetCapture(hwnd);
    break;
  }
  case WM_LBUTTONUP:
  {
    //ReleaseCapture();
    break;
  }

  {
    // Create the region from the client area.  
  //RECT rc;
  //POINTS ptTmp;
  //  GetClientRect(hwnd, &rc);
  //  hrgn = CreateRectRgn(rc.left, rc.top,
  //    rc.right, rc.bottom);

  //  ptTmp = MAKEPOINTS((POINTS FAR *) lParam);
  //  ptMouseDown[index].x = (LONG)ptTmp.x;
  //  ptMouseDown[index].y = (LONG)ptTmp.y;

  //  // Test for a hit in the client rectangle.  

  //  if (PtInRegion(hrgn, ptMouseDown[index].x,
  //    ptMouseDown[index].y))
  //  {
  //    // If a hit occurs, record the mouse coords.  

  //    Marker(ptMouseDown[index].x, ptMouseDown[index].y,
  //      hwnd);
  //    index++;
  //  }
  //}
  //break;
  }
  break;
  case WM_KEYDOWN:	     //按下键盘消息
               //判断按键的虚拟键码
    switch (wParam)
    {
    case VK_ESCAPE:           //按下【Esc】键
      DestroyWindow(hwnd);    // 销毁窗口, 并发送一条WM_DESTROY消息
      break;
    case VK_UP:				  //按下【↑】键
      break;
    case VK_DOWN:			  //按下【↓】键
      break;
    case VK_LEFT:			  //按下【←】键		
      break;
    case VK_RIGHT:			   //按下【→】键
      break;
    case VK_SPACE:
      break;
    }
    break;								//跳出该switch语句

  case WM_KEYUP:	     //按下键盘消息
             //判断按键的虚拟键码
    switch (wParam)
    {
    case VK_UP:				  //按下【↑】键
      break;
    case VK_DOWN:			  //按下【↓】键
      break;
    case VK_LEFT:			  //按下【←】键			
      break;
    case VK_RIGHT:			   //按下【→】键
      break;
    }
    break;
  case WM_DESTROY:					//若是窗口销毁消息
                    //Game_CleanUp(hwnd);			//调用自定义的资源清理函数Game_CleanUp（）进行退出前的资源清理
    PostQuitMessage(0);			//向系统表明有个线程有终止请求。用来响应WM_DESTROY消息
    break;									//跳出该switch语句

  default:										//若上述case条件都不符合，则执行该default语句


    break;
  }
  //这是调用lib的消息循环的地方
  if (callback_.windows_message) {
    if (callback_.windows_message(hwnd, message, wParam, lParam))
      return TRUE;
  }
  return  DefWindowProc(hwnd, message, wParam, lParam);		//调用缺省的窗口过程;									//正常退出
}
////////////////////////////////消息loop//////////////////////////////////////////////////////////
//无动画的消息的loop
void CWindowBsp::WindowMsgLoop() {
  // Main message loop:
  MSG msg = { 0 };				//定义并初始化msg
  BOOL bRet;//临时变量，存储GetMessage方法返回值
            // Main message loop:
  while ((0 < GetMessage(&msg, NULL, 0, 0)) != 0)//阻塞
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
}
//有动画的消息loop
void CWindowBsp::GameMsgLoop(/*REFRESHFN refreshFn*/) {
  MSG msg = { 0 };				//定义并初始化msg
  DWORD		tPre = 0, tNow = 0, tTim;
  while (msg.message != WM_QUIT)		//使用while循环，如果消息不是WM_QUIT消息，就继续循环
  {
    if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))   //查看应用程序消息队列，有消息时将队列中的消息派发出去。//不阻塞
    {
      TranslateMessage(&msg);		//将虚拟键消息转换为字符消息
      DispatchMessage(&msg);			//分发一个消息给窗口程序。
    }
    else
    {
      tNow = GetTickCount();   //获取当前系统时间
      tTim = tNow - tPre;
      if (tTim >= 20)        //当此次循环运行与上次绘图时间相差0.02秒时再进行重绘操作50hz
      {
        //重绘
        tPre = GetTickCount();   //获取当前系统时间
        Cartoon(tTim);
      }
    }

  }
}