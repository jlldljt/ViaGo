

//ͷ�ļ�
#include"../Win32Lib/WindowBsp.h"
#include <math.h>
#include "../Win32Lib/DrawingFactory.h"
#pragma comment(lib,"../Debug/Win32Lib.lib")
CDrawingFactory wins;
//������Ϣ������ �ӿ�
CWindowBsp windowbsp;
int x_mouse = -1, y_mouse = -1, move = 0;
HWND main_hwnd, child_hwnd1, child_hwnd2;
RECT rect;
POINT pt;
LRESULT LButtonDown(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
  if (hwnd == main_hwnd) {
    x_mouse = GET_X_LPARAM(lParam);// LOWORD(lParam);
    y_mouse = GET_Y_LPARAM(lParam); //HIWORD(lParam);
    GetClientRect(child_hwnd1, &rect);
    pt.x = (rect.right + rect.left) >> 1;
    pt.y = (rect.bottom + rect.top) >> 1;
    ClientToScreen(child_hwnd1, &pt);
    ScreenToClient(main_hwnd, &pt);
    move = 1;
  }
  return FALSE;
}

LRESULT Event(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
  if (hwnd == main_hwnd) {
    return wins.Event(hwnd, message, wParam, lParam);
  }
  return FALSE;
}
#define step_pixel 5 // per tick

void Animation(DWORD tick) {
  wins.Animation(tick);//ȱʡ����
}



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
  int				cxScreen, cyScreen;
  cxScreen = GetSystemMetrics(SM_CXSCREEN);
  cyScreen = GetSystemMetrics(SM_CYSCREEN);
  int width = 500;
  int height = 500;
  //RECT rect = { (cxScreen - width)/2, (cyScreen - height)/2, (cxScreen + width) / 2, (cyScreen + height) / 2 };
  RECT rect = { 100, 000, 300, 200 };
  RECT uprect = { 50, 10, 100, 50 };

  main_hwnd = windowbsp.CreateBoard(hInstance, L"hello act", rect, NULL);
  //windowbsp.ChangeIcon(hInstance, main_hwnd, L"pic/9.ico");
  //Ϊ����Ϣ�Ͷ���

  //����������Ԫ��
  wins.Bind(main_hwnd);
  //TODO:����������
  wins.Create({ 0,0,200,200 }, CDrawingFactory::_BLANK_);
  for (int i = 0; i < 10; i++) {
    InflateRect(&uprect, 1, 1);
    CDrawingBase *object = wins.Create(uprect, CDrawingFactory::_BUTTON_);
    TCHAR* img = TEXT("pic/3.PNG");
    //object->tools_->AddImage(img);
    object->AddImage(img);
    //RECT tmp_rect = {0, 0, uprect.right - uprect.left, uprect.bottom - uprect.top};
    //object->tools_->StretchBlt(tmp_rect, 0);
    //object->board_->EnableTransparent(true);

    object->ShowImage(0);
    object->EnableTransparent(true);
  }

  CDrawingBase *object = wins.Create(uprect, CDrawingFactory::_ANIMATION_BUTTON_);
  TCHAR* img = TEXT("pic/3.PNG");
  object->AddImage(img);
  img = TEXT("pic/5.PNG");
  object->AddImage(img);
  //RECT tmp_rect = { 0, 0, uprect.right - uprect.left, uprect.bottom - uprect.top };
  //object->tools_->StretchBlt(tmp_rect, 0);//��ʱ���������img���Ƶ�board�ϣ�ֻ����һ�μ��ɣ������Դ����棬����ÿ��paint����
  //object->board_->EnableTransparent(true);

  object->ShowImage(0);
  object->EnableTransparent(false);

  windowbsp.callback_.cartoon = Animation;//�û����庯����
  windowbsp.callback_.windows_message = Event;//�û����庯����


  InvalidateRect(main_hwnd, &uprect, 0);

  windowbsp.GameMsgLoop();// .WindowMsgLoop();
  windowbsp.DestoryBoard(hInstance, L"hello act");
  return 0;
}