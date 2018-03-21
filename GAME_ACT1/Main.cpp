
/*
 *	Main.cpp
 *	����win32��Ϸ��ܲ�����2d act��Ϸ����ٿ�
 */

#include "ACTEngine.h"
#include <windows.h>
#include <atlimage.h>	//CImage�������ͷ�ļ�
#include <tchar.h>//ʹ��swprintf_s���������ͷ�ļ�

#include <MMSystem.h>	//����ý��(����)���������ͷ�ļ�

#pragma comment(lib,"winmm.lib")			//����PlaySound����������ļ�
#pragma  comment(lib,"Msimg32.lib")		//���ʹ��TransparentBlt��������Ŀ��ļ�

#define WINDOW_WIDTH	800							//Ϊ���ڿ�ȶ���ĺ꣬�Է����ڴ˴��޸Ĵ��ڿ��
#define WINDOW_HEIGHT	600							//Ϊ���ڸ߶ȶ���ĺ꣬�Է����ڴ˴��޸Ĵ��ڸ߶�
#define WINDOW_TITLE		L"���ܰɣ��ֵ� "	//Ϊ���ڱ��ⶨ��ĺ�
#define WS_UNRESIZEWINDOW  (WS_OVERLAPPED     | \
	WS_CAPTION        | \
	WS_SYSMENU        | \
	WS_MINIMIZEBOX)
/************************************************************************/
/*                                ��ȫ�ֱ����������֡�                  */
/************************************************************************/
HDC				g_hdc=NULL,g_mdc=NULL,g_bufdc=NULL;      //ȫ���豸�������������ȫ���ڴ�DC���
HBITMAP		g_hSprite[4]={NULL},g_hBackGround=NULL;								//����λͼ����������ڴ洢���ŷ���ͼ���Լ�����洢����ͼ�ľ��
DWORD		g_tPre=0,g_tNow=0;					//����l������������¼ʱ��,g_tPre��¼��һ�λ�ͼ��ʱ�䣬g_tNow��¼�˴�׼����ͼ��ʱ��
int					g_iNum=0,g_iX=0,g_iY=0;                //g_iNum������¼ͼ�ţ�g_iX��g_iY�ֱ��ʾ��ͼ�ĺ�������
int					g_iDirection=0;//g_iDirectionΪ�����ƶ�����������������0��1��2��3���������ϣ��£����ҷ����ϵ��ƶ�

int         dirW = 0, dirH = 0 , posW,posH;
int         attack =0;

HWND		hwndChild[2];
HWND gameWnd=NULL;
CACTEngine *m_actEng = NULL;
/************************************************************************/
/*                                ��ȫ�ֺ����������֡�                  */
/************************************************************************/
LRESULT CALLBACK	WndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );//���ڹ��̺���
LRESULT CALLBACK ChildWndProc(HWND, UINT, WPARAM, LPARAM);	//�Ӵ��ڻص�����
LRESULT CALLBACK ActWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
//
void MoveCtl(void);
void AttackCtl(void);
/************************************************************************/
/*                        ��������ڵ㺯��WinMain��                     */
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
	////��1�����ڴ����Ĳ���֮һ����ʼ���һ�������Ĵ�����
	//WNDCLASSEX wndClass = { 0 };							//��WINDCLASSEX������һ��������
	//wndClass.cbSize = sizeof( WNDCLASSEX ) ;			//���ýṹ����ֽ�����С
	//wndClass.style = CS_HREDRAW | CS_VREDRAW;	//���ô��ڵ���ʽ
	//wndClass.lpfnWndProc = WndProc;					//����ָ�򴰿ڹ��̺�����ָ��
	//wndClass.cbClsExtra		= 0;								//������ĸ����ڴ棬ȡ0�Ϳ�����
	//wndClass.cbWndExtra		= 0;							//���ڵĸ����ڴ棬��Ȼȡ0������
	//wndClass.hInstance = hInstance;						//ָ���������ڹ��̵ĳ����ʵ�������
	//wndClass.hIcon=(HICON)::LoadImage(NULL,L"icon.ico",IMAGE_ICON,0,0,LR_DEFAULTSIZE|LR_LOADFROMFILE);  //���ؼ����Զ���icoͼ��//LoadIcon(NULL, IDI_APPLICATION);
	//wndClass.hCursor = LoadCursor( NULL, IDC_ARROW );    //ָ��������Ĺ������
	//wndClass.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);  //ΪhbrBackground��Աָ��һ����ɫ��ˢ���	
	//wndClass.lpszMenuName = NULL;						//��һ���Կ���ֹ���ַ�����ָ���˵���Դ�����֡�
	//wndClass.lpszClassName = L"ForTheDreamOfGameDevelop";		//��һ���Կ���ֹ���ַ�����ָ������������֡�

	////��2�����ڴ����Ĳ���֮����ע�ᴰ����
	//if( !RegisterClassEx( &wndClass ) )				//����괰�ں���Ҫ�Դ��������ע�ᣬ�������ܴ��������͵Ĵ���
	//{
	//	MessageBox(NULL, TEXT("This program requires Windows NT!"),
	//		 L"ForTheDreamOfGameDevelop", MB_ICONERROR);
	//	return -1;		
	//}

	//wndClass.lpfnWndProc	= ChildWndProc;
	//wndClass.hIcon			= NULL;
	//wndClass.lpszClassName	= L"childWindow";

	////ע���Ӵ�����
	//RegisterClassEx(&wndClass);

	//cxScreen = GetSystemMetrics(SM_CXSCREEN);
	//cyScreen = GetSystemMetrics(SM_CYSCREEN);

	////��3�����ڴ����Ĳ���֮������ʽ��������
	//HWND hwnd = CreateWindow( 
	//	L"ForTheDreamOfGameDevelop",
	//	WINDOW_TITLE,				//ϲ���ּ��Ĵ������ں���CreateWindow
	//	WS_OVERLAPPEDWINDOW, 
	//	CW_USEDEFAULT, //cxScreen >>2
	//	CW_USEDEFAULT, //cyScreen >>2
	//	WINDOW_WIDTH,
	//	WINDOW_HEIGHT, 
	//	NULL, 
	//	NULL, 
	//	hInstance, 
	//	NULL );

	////��4�����ڴ����Ĳ���֮�ģ����ڵ��ƶ�����ʾ�����
	//MoveWindow(hwnd,250,80,WINDOW_WIDTH,WINDOW_HEIGHT,true);		//����������ʾʱ��λ�ã�ʹ�������Ͻ�λ�ڣ�250,80����
	//ShowWindow( hwnd, nShowCmd );    //����ShowWindow��������ʾ����
	//UpdateWindow(hwnd);						//�Դ��ڽ��и��£��������������·���Ҫװ��һ��


	//
	////��Ϸ��Դ�ĳ�ʼ��������ʼ��ʧ�ܣ�����һ����Ϣ�򣬲�����FALSE
	////if (!Game_Init (hwnd)) 
	////{
	////	MessageBox(hwnd, L"��Դ��ʼ��ʧ��", L"��Ϣ����", 0); //ʹ��MessageBox����������һ����Ϣ����
	////	return FALSE;
	////}
	////PlaySound(L"bgm.mp3", NULL, SND_FILENAME | SND_ASYNC|SND_LOOP); //ѭ�����ű������� 
//	mciSendString(L"open bgm.mp3 alias bgm", 0, 0, 0);
//	mciSendString(L"play bgm repeat", 0, 0, 0);
//	CImage m_img;
//	m_img.Load(L"you-2.png");
//	//if (m_img.GetBPP() == 32) //ȷ�ϸ�ͼ�����Alphaͨ��
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
//		//AfxMessageBox(L"����ͼƬ����ʧ��!");
//		exit(0);
//	}
//
//	HDC hdc = GetDC(gameWnd);
//	HDC mdc = CreateCompatibleDC(hdc);  //����һ����hdc���ݵ��ڴ�DC
//	HBITMAP bmp = CreateCompatibleBitmap(hdc,WINDOW_WIDTH,WINDOW_HEIGHT);
//	SelectObject(mdc,bmp);
//	HDC bufdc = CreateCompatibleDC(hdc);//�ٴ���һ����hdc���ݵĻ���DC
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
//	//��5����Ϣѭ������
//	MSG msg = { 0 };				//���岢��ʼ��msg
//	while( msg.message != WM_QUIT )		//ʹ��whileѭ���������Ϣ����WM_QUIT��Ϣ���ͼ���ѭ��
//	{
//		if( PeekMessage( &msg, 0, 0, 0, PM_REMOVE ) )   //�鿴Ӧ�ó�����Ϣ���У�����Ϣʱ�������е���Ϣ�ɷ���ȥ��
//		{
//			TranslateMessage( &msg );		//���������Ϣת��Ϊ�ַ���Ϣ
//			DispatchMessage( &msg );			//�ַ�һ����Ϣ�����ڳ���
//		}
//		else
//		{
//			g_tNow = GetTickCount();   //��ȡ��ǰϵͳʱ��
//			if(g_tNow-g_tPre >= 50)        //���˴�ѭ���������ϴλ�ͼʱ�����0.05��ʱ�ٽ����ػ����
//			{
//				//�ػ�
//				g_tPre = GetTickCount();   //��ȡ��ǰϵͳʱ��
//				BitBlt(hdc,0,0,WINDOW_WIDTH,WINDOW_HEIGHT,mdc,0,0,SRCCOPY);
//				/*COLORREF clr = RGB(255,255,255);
//				INT A = m_img.GetTransparentColor();
//				m_img.SetTransparentColor(clr);*/
//				//m_img.Draw(hdc ,posW,posH,300,300,curW , curH , disW , disH);
//				HDC lhdcl = ::GetDC(hwndChild[0]);
//				m_img.TransparentBlt(lhdcl ,posW,posH,300,300,curW , curH , disW , disH,RGB(255,255,255));
//				ReleaseDC(hwndChild[0],lhdcl);
//				if(attack){
//				//��һ֡Ԥ�Ȼ���
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
//	//�ͷ���Դ����
//	/*DeleteObject(g_hBackGround);
//	DeleteDC(g_bufdc);
//	DeleteDC(g_mdc);
//	ReleaseDC(hwnd,g_hdc);
//	return TRUE;*/
//	//��6���������ע��
//	UnregisterClass(L"ForTheDreamOfGameDevelop", wndClass.hInstance);  //����׼��������ע��������
	delete m_actEng;
	return 0;  
}

//-----------------------------------��WndProc( )������--------------------------------------
//	���������ڹ��̺���WndProc,�Դ�����Ϣ���д���
//------------------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )      
{
	static int		cxClient, cyClient, cxBitmap, cyBitmap, cxChild, cyChild;
	//�Ӵ��ڵĴ��ھ��
	
	static HBITMAP	hBitmap;
	HINSTANCE		hInstance;
	HDC				hdc, hdcMem;
	PAINTSTRUCT		ps;
	int				i, j;
	switch( message )						//switch��俪ʼ
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
		//ȷ�������ڿͻ�����С
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);

		//ȷ���Ӵ��ڴ�С
		cxChild = cxClient / 4;
		cyChild = cyClient / 4;

		

		/*        �ƶ��Ӵ���      */
		/*for(i = 0; i < 2 ;i++)
		MoveWindow(hwndChild[i], i * (cxChild + 1),  (cyChild + 1),
				cxChild, cyChild, TRUE);*/
			break;

	//case WM_PAINT:		//����Paint��Ϣ�����ƴ��ڿͻ���
	//		//wnd.OnPaint();
	//		break;
	case WM_LBUTTONDOWN:
		{
			int xMouse, yMouse;
			xMouse = LOWORD(lParam);
			yMouse = HIWORD(lParam);

			for(i=0; i<2; i++)
				for(j=0; j<2; j++)
					//��������λ�����Ӵ����ϣ�����ʾ
					if((xMouse >= i * cxChild && xMouse < (i+1) * cxChild)
						&& (yMouse >= j * cyChild && yMouse < (j+1) * cyChild))
					{
						ShowWindow(hwndChild[i], SW_SHOW);
						return 0;
					}
		}
		break;
	case WM_KEYDOWN:	     //���¼�����Ϣ
		//�жϰ������������
		switch (wParam) 
		{
		case VK_ESCAPE:           //���¡�Esc����
			DestroyWindow(hwnd);    // ���ٴ���, ������һ��WM_DESTROY��Ϣ
			//PostQuitMessage( 0 );  //��������
			break;
		case VK_UP:				  //���¡�������
			//���ݰ������������ƶ�������ÿ�ΰ���һ�ΰ����ƶ�10����λ����������������ͼ�����X��Yֵ�������ж������Ƿ񳬳����������������������
			m_actEng->SetUp(5);
			
			break;
		case VK_DOWN:			  //���¡�������
			//m_actEng->SetDown(5);
			break;
		case VK_LEFT:			  //���¡�������			
			m_actEng->SetLeft(5);	
			break;
		case VK_RIGHT:			   //���¡�������
			m_actEng->SetRight(5);
			break;
		case VK_SPACE:
			attack = 1;
		}
		MoveCtl();
		AttackCtl();
		break;								//������switch���

	case WM_KEYUP:	     //���¼�����Ϣ
		//�жϰ������������
		switch (wParam) 
		{
		case VK_UP:				  //���¡�������
			//���ݰ������������ƶ�������ÿ�ΰ���һ�ΰ����ƶ�10����λ����������������ͼ�����X��Yֵ�������ж������Ƿ񳬳����������������������
			m_actEng->ResetU();
			break;
		case VK_DOWN:			  //���¡�������
			//m_actEng->ResetD();
			break;
		case VK_LEFT:			  //���¡�������			
			m_actEng->ResetL();	
			break;
		case VK_RIGHT:			   //���¡�������
			m_actEng->ResetR();
			break;
		}			
		MoveCtl();
		AttackCtl();
		break;	
	case WM_DESTROY:					//���Ǵ���������Ϣ
		//Game_CleanUp(hwnd);			//�����Զ������Դ������Game_CleanUp���������˳�ǰ����Դ����
		PostQuitMessage( 0 );			//��ϵͳ�����и��߳�����ֹ����������ӦWM_DESTROY��Ϣ
		break;									//������switch���

	default:										//������case�����������ϣ���ִ�и�default���
		return DefWindowProc( hwnd, message, wParam, lParam );		//����ȱʡ�Ĵ��ڹ���
	}

	return 0;									//�����˳�
}

//�Ӵ��ڻص�����
LRESULT CALLBACK ChildWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		//����Ӵ���ʱ����
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
		//����Ӵ���ʱ����
	default:
		break;
	}
	return DefWindowProc (hwnd, message, wParam, lParam);
}

/************************************************************************/
/* �Զ��庯��                                                           */
/************************************************************************/
void MoveCtl(void) {
	switch ( m_actEng->CheckMove())
	{
	case 5:
		if (1 == m_actEng->GetHumanAct()) {
			m_actEng->ChangeHumanAct(0,-1);//������һ�η���
		}
		break;
	case 1:
	case 4:
	case 7:
		if (0 == m_actEng->GetHumanAct() || 1 == m_actEng->GetHumanAct()) {
			m_actEng->ChangeHumanAct(1,1);//��ת����
			m_actEng->SetScreenRefresh(true);
		}
		break;
	case 2:
	case 8:
		if (0 == m_actEng->GetHumanAct() || 1 == m_actEng->GetHumanAct()) {
			m_actEng->ChangeHumanAct(1,-1);//������һ�η���
			m_actEng->SetScreenRefresh(true);
		}
		break;
	case 3:
	case 6:
	case 9:
		if (0 == m_actEng->GetHumanAct() || 1 == m_actEng->GetHumanAct()) {
			m_actEng->ChangeHumanAct(1,0);//����ת����
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
			m_actEng->ChangeHumanAct(2,-1);//������һ�η���
			m_actEng->SetScreenRefresh(false);
		}
		break;
	default:
		break;
	}
	attack = 0;
}