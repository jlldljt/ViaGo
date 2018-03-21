#include "WindowBsp.h"
#include <assert.h>
//�����ǻص������������ɾ�̬������ʵ�Ҳ�����Ǿ�̬������̬��CallBackFunction������⡣������ȫ��
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
*	����w32������
*/
HWND CWindowBsp::CreateWin32Window(
  HINSTANCE hInstance,
  LPCWSTR lpName,
  DWORD dwStyle,
  HWND hWndParent) {

  //��1�����ڴ����Ĳ���֮һ����ʼ���һ�������Ĵ�����

  WNDCLASSEX wndClass = { 0 };							//��WINDCLASSEX������һ��������
  wndClass.cbSize = sizeof(WNDCLASSEX);			//���ýṹ����ֽ�����С
  wndClass.style = CS_HREDRAW | CS_VREDRAW;	//���ô��ڵ���ʽ
  wndClass.lpfnWndProc = WndProc;// (proc_.wnd_proc);					//����ָ�򴰿ڹ��̺�����ָ��
  wndClass.cbClsExtra = 0;								//������ĸ����ڴ棬ȡ0�Ϳ�����
  wndClass.cbWndExtra = 0;							//���ڵĸ����ڴ棬��Ȼȡ0������
  wndClass.hInstance = hInstance;						//ָ���������ڹ��̵ĳ����ʵ�������
 wndClass.hIcon = (HICON)::LoadImage(NULL, L"5.jpg", IMAGE_BITMAP,0,0,LR_DEFAULTSIZE|LR_LOADFROMFILE);  //���ؼ����Զ���icoͼ��//LoadIcon(NULL, IDI_APPLICATION);
 
  //wndClass.hIcon = //(HICON)::LoadImage(NULL,L"icon.ico",IMAGE_ICON,0,0,LR_DEFAULTSIZE|LR_LOADFROMFILE);  //���ؼ����Զ���icoͼ��//LoadIcon(NULL, IDI_APPLICATION);
  wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);    //ָ��������Ĺ������
  wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);  //ΪhbrBackground��Աָ��һ����ɫ��ˢ���	
  wndClass.lpszMenuName = NULL;						//��һ���Կ���ֹ���ַ�����ָ���˵���Դ�����֡�
  wndClass.lpszClassName = lpName;		//��һ���Կ���ֹ���ַ�����ָ������������֡�

                      //��2�����ڴ����Ĳ���֮����ע�ᴰ����
  if (!RegisterClassEx(&wndClass))				//����괰�ں���Ҫ�Դ��������ע�ᣬ�������ܴ��������͵Ĵ���
  {
    MessageBox(NULL, TEXT("This program requires Windows NT!"),
      lpName, MB_ICONERROR);
    return NULL;
  }

  //��3�����ڴ����Ĳ���֮������ʽ��������
  HWND hwnd = CreateWindow(
    lpName,
    NULL,				//���ڵı��⣬Ĭ��û�У����Ժ�����������
    dwStyle,
    0,
    0,
    0,
    0,
    hWndParent,
    NULL,//�Ӵ��ڣ�����Ҫ��ID��������û��IDò��Ҳû�£�ֻ�ǲ��������й�ID��һЩ������
    hInstance,
    NULL);

  //��4�����ڴ����Ĳ���֮�ģ����ڵ��ƶ�����ʾ�����
  //MoveWindow(hwnd,cxScreen >>2,cyScreen >>2,cxScreen >>1,	cyScreen >>1, true);		//����������ʾʱ��λ�ã�ʹ�������Ͻ�λ�ڣ�250,80����
  //ShowWindow( hwnd, true );    //����ShowWindow��������ʾ����
  //UpdateWindow(hwnd);						//�Դ��ڽ��и��£��������������·���Ҫװ��һ��

  return hwnd;
}

/************************************************************************/
/*          CACTWindow                                                  */
/************************************************************************/
/*
*	���������Ӵ���
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

  MoveWindow(hwnd, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, true);		//����������ʾʱ��λ�ã�ʹ�������Ͻ�λ�ڣ�250,80����
  ShowWindow(hwnd, true);    //����ShowWindow��������ʾ����
  UpdateWindow(hwnd);						//�Դ��ڽ��и��£��������������·���Ҫװ��һ��

  //m_instance = hInstance;
  //m_name = lpName;
  //hwnd_ = hwnd;
  return hwnd;
}

/*
*	����������
*/
bool CWindowBsp::DestoryBoard(
  HINSTANCE hInstance,
  LPCWSTR lpName) {
  //��6���������ע��
  UnregisterClass(lpName, hInstance);  //����׼��������ע��������
  return 0;
}

//tick���������ms
void CWindowBsp::Cartoon(DWORD tick)
{
  if (callback_.cartoon)
    callback_.cartoon(tick);
}
///////////////////////////////���ڹ��̣���������Ϣ//////////////////////////////////////////////
LRESULT CALLBACK CWindowBsp::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  static int		cxClient, cyClient, cxBitmap, cyBitmap, cxChild, cyChild;
  //�Ӵ��ڵĴ��ھ��

  static HBITMAP	hBitmap;
  HINSTANCE		hInstance;
  HDC				hdc, hdcMem;
  PAINTSTRUCT		ps;
  int				i, j;

  switch (message)						//switch��俪ʼ
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
  case WM_KEYDOWN:	     //���¼�����Ϣ
               //�жϰ������������
    switch (wParam)
    {
    case VK_ESCAPE:           //���¡�Esc����
      DestroyWindow(hwnd);    // ���ٴ���, ������һ��WM_DESTROY��Ϣ
      break;
    case VK_UP:				  //���¡�������
      break;
    case VK_DOWN:			  //���¡�������
      break;
    case VK_LEFT:			  //���¡�������		
      break;
    case VK_RIGHT:			   //���¡�������
      break;
    case VK_SPACE:
      break;
    }
    break;								//������switch���

  case WM_KEYUP:	     //���¼�����Ϣ
             //�жϰ������������
    switch (wParam)
    {
    case VK_UP:				  //���¡�������
      break;
    case VK_DOWN:			  //���¡�������
      break;
    case VK_LEFT:			  //���¡�������			
      break;
    case VK_RIGHT:			   //���¡�������
      break;
    }
    break;
  case WM_DESTROY:					//���Ǵ���������Ϣ
                    //Game_CleanUp(hwnd);			//�����Զ������Դ������Game_CleanUp���������˳�ǰ����Դ����
    PostQuitMessage(0);			//��ϵͳ�����и��߳�����ֹ����������ӦWM_DESTROY��Ϣ
    break;									//������switch���

  default:										//������case�����������ϣ���ִ�и�default���


    break;
  }
  //���ǵ���lib����Ϣѭ���ĵط�
  if (callback_.windows_message) {
    if (callback_.windows_message(hwnd, message, wParam, lParam))
      return TRUE;
  }
  return  DefWindowProc(hwnd, message, wParam, lParam);		//����ȱʡ�Ĵ��ڹ���;									//�����˳�
}
////////////////////////////////��Ϣloop//////////////////////////////////////////////////////////
//�޶�������Ϣ��loop
void CWindowBsp::WindowMsgLoop() {
  // Main message loop:
  MSG msg = { 0 };				//���岢��ʼ��msg
  BOOL bRet;//��ʱ�������洢GetMessage��������ֵ
            // Main message loop:
  while ((0 < GetMessage(&msg, NULL, 0, 0)) != 0)//����
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
}
//�ж�������Ϣloop
void CWindowBsp::GameMsgLoop(/*REFRESHFN refreshFn*/) {
  MSG msg = { 0 };				//���岢��ʼ��msg
  DWORD		tPre = 0, tNow = 0, tTim;
  while (msg.message != WM_QUIT)		//ʹ��whileѭ���������Ϣ����WM_QUIT��Ϣ���ͼ���ѭ��
  {
    if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))   //�鿴Ӧ�ó�����Ϣ���У�����Ϣʱ�������е���Ϣ�ɷ���ȥ��//������
    {
      TranslateMessage(&msg);		//���������Ϣת��Ϊ�ַ���Ϣ
      DispatchMessage(&msg);			//�ַ�һ����Ϣ�����ڳ���
    }
    else
    {
      tNow = GetTickCount();   //��ȡ��ǰϵͳʱ��
      tTim = tNow - tPre;
      if (tTim >= 20)        //���˴�ѭ���������ϴλ�ͼʱ�����0.02��ʱ�ٽ����ػ����50hz
      {
        //�ػ�
        tPre = GetTickCount();   //��ȡ��ǰϵͳʱ��
        Cartoon(tTim);
      }
    }

  }
}