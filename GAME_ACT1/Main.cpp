
/*
 *	Main.cpp
 *	尝试win32游戏框架并建立2d act游戏人物操控
 */

#include "ACTEngine.h"
#include <windows.h>
#include <atlimage.h>	//CImage类包含的头文件
#include <tchar.h>//使用swprintf_s函数所需的头文件

#include <MMSystem.h>	//播放媒体(音乐)所需包含的头文件

#pragma comment(lib,"winmm.lib")			//调用PlaySound函数所需库文件
#pragma  comment(lib,"Msimg32.lib")		//添加使用TransparentBlt函数所需的库文件

#define WINDOW_WIDTH	800							//为窗口宽度定义的宏，以方便在此处修改窗口宽度
#define WINDOW_HEIGHT	600							//为窗口高度定义的宏，以方便在此处修改窗口高度
#define WINDOW_TITLE		L"奔跑吧，兄弟 "	//为窗口标题定义的宏
#define WS_UNRESIZEWINDOW  (WS_OVERLAPPED     | \
	WS_CAPTION        | \
	WS_SYSMENU        | \
	WS_MINIMIZEBOX)
/************************************************************************/
/*                                【全局变量声明部分】                  */
/************************************************************************/
HDC				g_hdc=NULL,g_mdc=NULL,g_bufdc=NULL;      //全局设备环境句柄与两个全局内存DC句柄
HBITMAP		g_hSprite[4]={NULL},g_hBackGround=NULL;								//定义位图句柄数组用于存储四张方向图，以及定义存储背景图的句柄
DWORD		g_tPre=0,g_tNow=0;					//声明l两个函数来记录时间,g_tPre记录上一次绘图的时间，g_tNow记录此次准备绘图的时间
int					g_iNum=0,g_iX=0,g_iY=0;                //g_iNum用来记录图号，g_iX，g_iY分别表示贴图的横纵坐标
int					g_iDirection=0;//g_iDirection为人物移动方向，这里我们中以0，1，2，3代表人物上，下，左，右方向上的移动

int         dirW = 0, dirH = 0 , posW,posH;
int         attack =0;

HWND		hwndChild[2];
HWND gameWnd=NULL;
CACTEngine *m_actEng = NULL;
/************************************************************************/
/*                                【全局函数声明部分】                  */
/************************************************************************/
LRESULT CALLBACK	WndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );//窗口过程函数
LRESULT CALLBACK ChildWndProc(HWND, UINT, WPARAM, LPARAM);	//子窗口回调函数
LRESULT CALLBACK ActWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
//
void MoveCtl(void);
void AttackCtl(void);
/************************************************************************/
/*                        【程序入口点函数WinMain】                     */
/************************************************************************/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nShowCmd)
{
	//HWND tmpHwnd = m_ACTEngine.CreateMainWindow(hInstance, L"hello act", NULL,ActWndProc,0L);
	int				cxScreen, cyScreen;
	cxScreen = GetSystemMetrics(SM_CXSCREEN);
	cyScreen = GetSystemMetrics(SM_CYSCREEN);
	m_actEng = new CACTEngine(hInstance, L"hello act", cxScreen>>2 , cyScreen>>2 , cxScreen>>1 , cyScreen>>1 , NULL,WndProc,0L);
	gameWnd = m_actEng->m_window->CreateChildWindow(m_actEng->m_actWnd,L"child one", 100, 100, 200, 200, ActWndProc,0L);
	//RECT rc{ 0, 0,  cxScreen >> 1 , cyScreen >> 1 };
	m_actEng->AddScene(/*L"you-2.png"*/L"bki.bmp" , L"bkib1.bmp");
	m_actEng->AddHuman(L"you.png",L"human.txt");
	m_actEng->SetHumanZoom(1);
	/*CImage cimg;
	cimg.Load(L"you-2.png");
	HDC lhdcl = ::GetDC(gameWnd);
	cimg.TransparentBlt(lhdcl ,0,0,100,100,0 , 0 , cimg.GetWidth() , cimg.GetHeight(),RGB(255,255,255));*/



	m_actEng->ActRun();
	////【1】窗口创建四步曲之一：开始设计一个完整的窗口类
	//WNDCLASSEX wndClass = { 0 };							//用WINDCLASSEX定义了一个窗口类
	//wndClass.cbSize = sizeof( WNDCLASSEX ) ;			//设置结构体的字节数大小
	//wndClass.style = CS_HREDRAW | CS_VREDRAW;	//设置窗口的样式
	//wndClass.lpfnWndProc = WndProc;					//设置指向窗口过程函数的指针
	//wndClass.cbClsExtra		= 0;								//窗口类的附加内存，取0就可以了
	//wndClass.cbWndExtra		= 0;							//窗口的附加内存，依然取0就行了
	//wndClass.hInstance = hInstance;						//指定包含窗口过程的程序的实例句柄。
	//wndClass.hIcon=(HICON)::LoadImage(NULL,L"icon.ico",IMAGE_ICON,0,0,LR_DEFAULTSIZE|LR_LOADFROMFILE);  //本地加载自定义ico图标//LoadIcon(NULL, IDI_APPLICATION);
	//wndClass.hCursor = LoadCursor( NULL, IDC_ARROW );    //指定窗口类的光标句柄。
	//wndClass.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);  //为hbrBackground成员指定一个白色画刷句柄	
	//wndClass.lpszMenuName = NULL;						//用一个以空终止的字符串，指定菜单资源的名字。
	//wndClass.lpszClassName = L"ForTheDreamOfGameDevelop";		//用一个以空终止的字符串，指定窗口类的名字。

	////【2】窗口创建四步曲之二：注册窗口类
	//if( !RegisterClassEx( &wndClass ) )				//设计完窗口后，需要对窗口类进行注册，这样才能创建该类型的窗口
	//{
	//	MessageBox(NULL, TEXT("This program requires Windows NT!"),
	//		 L"ForTheDreamOfGameDevelop", MB_ICONERROR);
	//	return -1;		
	//}

	//wndClass.lpfnWndProc	= ChildWndProc;
	//wndClass.hIcon			= NULL;
	//wndClass.lpszClassName	= L"childWindow";

	////注册子窗口类
	//RegisterClassEx(&wndClass);

	//cxScreen = GetSystemMetrics(SM_CXSCREEN);
	//cyScreen = GetSystemMetrics(SM_CYSCREEN);

	////【3】窗口创建四步曲之三：正式创建窗口
	//HWND hwnd = CreateWindow( 
	//	L"ForTheDreamOfGameDevelop",
	//	WINDOW_TITLE,				//喜闻乐见的创建窗口函数CreateWindow
	//	WS_OVERLAPPEDWINDOW, 
	//	CW_USEDEFAULT, //cxScreen >>2
	//	CW_USEDEFAULT, //cyScreen >>2
	//	WINDOW_WIDTH,
	//	WINDOW_HEIGHT, 
	//	NULL, 
	//	NULL, 
	//	hInstance, 
	//	NULL );

	////【4】窗口创建四步曲之四：窗口的移动、显示与更新
	//MoveWindow(hwnd,250,80,WINDOW_WIDTH,WINDOW_HEIGHT,true);		//调整窗口显示时的位置，使窗口左上角位于（250,80）处
	//ShowWindow( hwnd, nShowCmd );    //调用ShowWindow函数来显示窗口
	//UpdateWindow(hwnd);						//对窗口进行更新，就像我们买了新房子要装修一样


	//
	////游戏资源的初始化，若初始化失败，弹出一个消息框，并返回FALSE
	////if (!Game_Init (hwnd)) 
	////{
	////	MessageBox(hwnd, L"资源初始化失败", L"消息窗口", 0); //使用MessageBox函数，创建一个消息窗口
	////	return FALSE;
	////}
	////PlaySound(L"bgm.mp3", NULL, SND_FILENAME | SND_ASYNC|SND_LOOP); //循环播放背景音乐 
//	mciSendString(L"open bgm.mp3 alias bgm", 0, 0, 0);
//	mciSendString(L"play bgm repeat", 0, 0, 0);
//	CImage m_img;
//	m_img.Load(L"you-2.png");
//	//if (m_img.GetBPP() == 32) //确认该图像包含Alpha通道
//	//{ 
//	//	int i;
//	//	int j;
//	//	for (i=0; i<m_img.GetWidth(); i++)
//	//	{
//	//		for (j=0; j<m_img.GetHeight(); j++)
//	//		{
//	//			byte *pByte = (byte *)m_img.GetPixelAddress(i, j);
//	//			pByte[0] = pByte[0] * pByte[3] / 255;
//	//			pByte[1] = pByte[1] * pByte[3] / 255;
//	//			pByte[2] = pByte[2] * pByte[3] / 255;
//	//		}
//	//	}
//	//}
//	if(m_img.IsNull())
//	{
//		//AfxMessageBox(L"背景图片加载失败!");
//		exit(0);
//	}
//
//	HDC hdc = GetDC(gameWnd);
//	HDC mdc = CreateCompatibleDC(hdc);  //创建一个和hdc兼容的内存DC
//	HBITMAP bmp = CreateCompatibleBitmap(hdc,WINDOW_WIDTH,WINDOW_HEIGHT);
//	SelectObject(mdc,bmp);
//	HDC bufdc = CreateCompatibleDC(hdc);//再创建一个和hdc兼容的缓冲DC
//	HBITMAP hBit = g_hBackGround = (HBITMAP)LoadImage(NULL,L"bki.bmp",IMAGE_BITMAP,800,600,LR_LOADFROMFILE);
//	SelectObject(bufdc,hBit);
//	SelectObject(bufdc,g_hBackGround);
//	int g_iBGOffset = 100;
//	BitBlt(mdc,0,0,g_iBGOffset,WINDOW_HEIGHT,bufdc,WINDOW_WIDTH-g_iBGOffset,0,SRCCOPY);
//	BitBlt(mdc,g_iBGOffset,0,WINDOW_WIDTH-g_iBGOffset,WINDOW_HEIGHT,bufdc,0,0,SRCCOPY);
//	
//	/*m_img.TransparentBlt(hdc,
//	0, 0,
//	600, 600,
//	RGB(255, 255, 255));*/
//	RECT rect;
//	GetClientRect(gameWnd,&rect);
//	//m_img.Draw(hdc,	rect);
//	
//	int wth = m_img.GetWidth();
//	int hei = m_img.GetHeight();
//	int disW = wth/5;
//	int disH = 190;//hei/7;
//	int curW = 0,curH = 0,curDW = 0;
//	int cnt = 0;
//
//
//	//【5】消息循环过程
//	MSG msg = { 0 };				//定义并初始化msg
//	while( msg.message != WM_QUIT )		//使用while循环，如果消息不是WM_QUIT消息，就继续循环
//	{
//		if( PeekMessage( &msg, 0, 0, 0, PM_REMOVE ) )   //查看应用程序消息队列，有消息时将队列中的消息派发出去。
//		{
//			TranslateMessage( &msg );		//将虚拟键消息转换为字符消息
//			DispatchMessage( &msg );			//分发一个消息给窗口程序。
//		}
//		else
//		{
//			g_tNow = GetTickCount();   //获取当前系统时间
//			if(g_tNow-g_tPre >= 50)        //当此次循环运行与上次绘图时间相差0.05秒时再进行重绘操作
//			{
//				//重绘
//				g_tPre = GetTickCount();   //获取当前系统时间
//				BitBlt(hdc,0,0,WINDOW_WIDTH,WINDOW_HEIGHT,mdc,0,0,SRCCOPY);
//				/*COLORREF clr = RGB(255,255,255);
//				INT A = m_img.GetTransparentColor();
//				m_img.SetTransparentColor(clr);*/
//				//m_img.Draw(hdc ,posW,posH,300,300,curW , curH , disW , disH);
//				HDC lhdcl = ::GetDC(hwndChild[0]);
//				m_img.TransparentBlt(lhdcl ,posW,posH,300,300,curW , curH , disW , disH,RGB(255,255,255));
//				ReleaseDC(hwndChild[0],lhdcl);
//				if(attack){
//				//下一帧预先绘制
//				curW+=disW;
//				cnt ++;
//				
//				
//				if (cnt > 12)
//				{
//					HDC lhdc = ::GetDC(hwndChild[1]);
//					m_img.Draw(lhdc ,/*300+posW*/0,/*posH*/0,100,100,curW+disW , curH , disW , disH);
//					ReleaseDC(hwndChild[1],lhdc);
//					curW = 0;
//					curH = 0;
//					cnt = 0;
//					attack = 0;
//				}
//				else{
//					if (curW >= wth)
//					{
//						curW = 0;
//						curH += disH;
//						if (curH >= hei)
//						{
//							curH = 0;
//							cnt = 0;
//							attack = 0;
//						}
//					}
//				}
//	}
//				
//				if (posH < hei || posH > 0)
//				{
//posH += dirH ;
//				}
//				if (posW < wth || posW > 0)
//				posW += dirW ;
//			
//			}
//		}
//
//	}
//	//释放资源对象
//	/*DeleteObject(g_hBackGround);
//	DeleteDC(g_bufdc);
//	DeleteDC(g_mdc);
//	ReleaseDC(hwnd,g_hdc);
//	return TRUE;*/
//	//【6】窗口类的注销
//	UnregisterClass(L"ForTheDreamOfGameDevelop", wndClass.hInstance);  //程序准备结束，注销窗口类
	delete m_actEng;
	return 0;  
}

//-----------------------------------【WndProc( )函数】--------------------------------------
//	描述：窗口过程函数WndProc,对窗口消息进行处理
//------------------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )      
{
	static int		cxClient, cyClient, cxBitmap, cyBitmap, cxChild, cyChild;
	//子窗口的窗口句柄
	
	static HBITMAP	hBitmap;
	HINSTANCE		hInstance;
	HDC				hdc, hdcMem;
	PAINTSTRUCT		ps;
	int				i, j;
	switch( message )						//switch语句开始
	{
		
	case WM_CREATE:
		/*BITMAP			bitmap;

		hInstance	= ((LPCREATESTRUCT)lParam)->hInstance;
		hBitmap		= LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP1));

		GetObject(hBitmap, sizeof(BITMAP), &bitmap);

		cxBitmap	= bitmap.bmWidth;
		cyBitmap	= bitmap.bmHeight;*/

		hInstance	= ((LPCREATESTRUCT)lParam)->hInstance;
		
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

	//case WM_PAINT:		//窗口Paint消息：绘制窗口客户区
	//		//wnd.OnPaint();
	//		break;
	case WM_LBUTTONDOWN:
		{
			int xMouse, yMouse;
			xMouse = LOWORD(lParam);
			yMouse = HIWORD(lParam);

			for(i=0; i<2; i++)
				for(j=0; j<2; j++)
					//如果鼠标点击位置在子窗口上，就显示
					if((xMouse >= i * cxChild && xMouse < (i+1) * cxChild)
						&& (yMouse >= j * cyChild && yMouse < (j+1) * cyChild))
					{
						ShowWindow(hwndChild[i], SW_SHOW);
						return 0;
					}
		}
		break;
	case WM_KEYDOWN:	     //按下键盘消息
		//判断按键的虚拟键码
		switch (wParam) 
		{
		case VK_ESCAPE:           //按下【Esc】键
			DestroyWindow(hwnd);    // 销毁窗口, 并发送一条WM_DESTROY消息
			//PostQuitMessage( 0 );  //结束程序
			break;
		case VK_UP:				  //按下【↑】键
			//根据按键加入人物移动的量（每次按下一次按键移动10个单位），来决定人物贴图坐标的X与Y值，接着判断坐标是否超出窗口区域，若有则进行修正
			m_actEng->SetUp(5);
			
			break;
		case VK_DOWN:			  //按下【↓】键
			//m_actEng->SetDown(5);
			break;
		case VK_LEFT:			  //按下【←】键			
			m_actEng->SetLeft(5);	
			break;
		case VK_RIGHT:			   //按下【→】键
			m_actEng->SetRight(5);
			break;
		case VK_SPACE:
			attack = 1;
		}
		MoveCtl();
		AttackCtl();
		break;								//跳出该switch语句

	case WM_KEYUP:	     //按下键盘消息
		//判断按键的虚拟键码
		switch (wParam) 
		{
		case VK_UP:				  //按下【↑】键
			//根据按键加入人物移动的量（每次按下一次按键移动10个单位），来决定人物贴图坐标的X与Y值，接着判断坐标是否超出窗口区域，若有则进行修正
			m_actEng->ResetU();
			break;
		case VK_DOWN:			  //按下【↓】键
			//m_actEng->ResetD();
			break;
		case VK_LEFT:			  //按下【←】键			
			m_actEng->ResetL();	
			break;
		case VK_RIGHT:			   //按下【→】键
			m_actEng->ResetR();
			break;
		}			
		MoveCtl();
		AttackCtl();
		break;	
	case WM_DESTROY:					//若是窗口销毁消息
		//Game_CleanUp(hwnd);			//调用自定义的资源清理函数Game_CleanUp（）进行退出前的资源清理
		PostQuitMessage( 0 );			//向系统表明有个线程有终止请求。用来响应WM_DESTROY消息
		break;									//跳出该switch语句

	default:										//若上述case条件都不符合，则执行该default语句
		return DefWindowProc( hwnd, message, wParam, lParam );		//调用缺省的窗口过程
	}

	return 0;									//正常退出
}

//子窗口回调函数
LRESULT CALLBACK ChildWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		//点击子窗口时隐藏
	case WM_LBUTTONDOWN:
		ShowWindow(hwnd, SW_HIDE);
		return 0;
	}
	return DefWindowProc (hwnd, message, wParam, lParam);
}

LRESULT CALLBACK ActWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		//点击子窗口时隐藏
	default:
		break;
	}
	return DefWindowProc (hwnd, message, wParam, lParam);
}

/************************************************************************/
/* 自定义函数                                                           */
/************************************************************************/
void MoveCtl(void) {
	switch ( m_actEng->CheckMove())
	{
	case 5:
		if (1 == m_actEng->GetHumanAct()) {
			m_actEng->ChangeHumanAct(0,-1);//保持上一次方向
		}
		break;
	case 1:
	case 4:
	case 7:
		if (0 == m_actEng->GetHumanAct() || 1 == m_actEng->GetHumanAct()) {
			m_actEng->ChangeHumanAct(1,1);//翻转方向
			m_actEng->SetScreenRefresh(true);
		}
		break;
	case 2:
	case 8:
		if (0 == m_actEng->GetHumanAct() || 1 == m_actEng->GetHumanAct()) {
			m_actEng->ChangeHumanAct(1,-1);//保持上一次方向
			m_actEng->SetScreenRefresh(true);
		}
		break;
	case 3:
	case 6:
	case 9:
		if (0 == m_actEng->GetHumanAct() || 1 == m_actEng->GetHumanAct()) {
			m_actEng->ChangeHumanAct(1,0);//不翻转方向
			m_actEng->SetScreenRefresh(true);
		}
		break;
	default:
		break;
	}
}

void AttackCtl(void) {
	switch (attack)
	{
	case 1:
		if (0 == m_actEng->GetHumanAct() || 1 == m_actEng->GetHumanAct()) {
			m_actEng->ChangeHumanAct(2,-1);//保持上一次方向
			m_actEng->SetScreenRefresh(false);
		}
		break;
	default:
		break;
	}
	attack = 0;
}