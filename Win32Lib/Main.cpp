

//头文件
#include"WindowBsp.h"
#include <math.h>
#include "DrawingFactory.h"
CDrawingFactory wins;
//测试消息、动画 接口
CWindowBsp windowbsp;
int x_mouse = -1, y_mouse = -1,move = 0;
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
//void Animation(DWORD tick) {
//  if (move/*x_mouse != -1 && y_mouse != -1*/) {
//   
//    int width = rect.right - rect.left + 2;
//    int height = rect.bottom - rect.top + 2;
//    int midx = pt.x;
//    int midy = pt.y;
//    int offset_x, offset_y;
//    if (x_mouse > midx) {
//      offset_x = step_pixel;// *tick;
//      if (midx + offset_x > x_mouse) {
//        offset_x = x_mouse - midx;
//       // x_mouse = -1;
//      }
//    }
//    else {
//      offset_x = -step_pixel;// *tick;
//      if (midx + offset_x < x_mouse) {
//        offset_x = x_mouse - midx;
//       // x_mouse = -1;
//      }
//    }
//
//    if (y_mouse > midy) {
//      offset_y = step_pixel;// *tick;
//      if (midy + offset_y > y_mouse) {
//        offset_y = y_mouse - midy;
//       // y_mouse = -1;
//      }
//    }
//    else {
//      offset_y = -step_pixel;// *tick;
//      if (midy + offset_y < y_mouse) {
//        offset_y = y_mouse - midy;
//      //  y_mouse = -1;
//      }
//    }
//    ShowWindow(child_hwnd1, false);    //调用ShowWindow函数来显示窗口
//    MoveWindow(child_hwnd1, midx - width/2 + offset_x, midy - height/2 + offset_y, width, height, TRUE);
//    ShowWindow(child_hwnd1, true);    //调用ShowWindow函数来显示窗口
//    UpdateWindow(child_hwnd1);						//对窗口进行更新，就像我们买了新房子要装修一样
//    pt.x += offset_x;
//    pt.y += offset_y;
//  }
//  else
//  {
//    
//  }
//
//
//}
void Animation(DWORD tick) {
  wins.Animation(tick);//缺省动作
}

/************************************************************************/
/*                        【程序入口点函数WinMain】                     */
/************************************************************************/
//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
//{
//	//HWND tmpHwnd = m_ACTEngine.CreateMainWindow(hInstance, L"hello act", NULL,ActWndProc,0L);
//	int				cxScreen, cyScreen;
//	cxScreen = GetSystemMetrics(SM_CXSCREEN);
//	cyScreen = GetSystemMetrics(SM_CYSCREEN);
//	RECT rect = {100, 100, cxScreen >>1, cyScreen >>1};
//	main_hwnd = windowbsp.CreateBoard(hInstance, L"hello act", rect,NULL);
//  rect = { 0, 0, 100, 100};
//	child_hwnd1 = windowbsp.CreateBoard(hInstance, L"child one", rect, main_hwnd);
// // SetBkColor(GetDC(child_hwnd1), 0x00ccdd);
//  rect = { 200, 150, 300, 250 };
//  child_hwnd2 = windowbsp.CreateBoard(hInstance, L"child two", rect, main_hwnd);
// 
//  windowbsp.callback_->cartoon = Animation;
//  windowbsp.callback_->lbuttondown = LButtonDown;
//
//  windowbsp.GameMsgLoop();// .WindowMsgLoop();
//
//
//	return 0;
//}



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
 //为了消息和动画
  
  //创建画布及元素
  wins.Bind(main_hwnd);
  //TODO:这里有问题
  wins.Create({0,0,200,200}, CDrawingFactory::_BLANK_);
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
  //object->tools_->StretchBlt(tmp_rect, 0);//临时，在这里把img绘制到board上，只绘制一次即可，由于自带缓存，无需每次paint都绘
  //object->board_->EnableTransparent(true);

  object->ShowImage(0);
  object->EnableTransparent(false);

  windowbsp.callback_.cartoon = Animation;//用户定义函数？
  windowbsp.callback_.windows_message = Event;//用户定义函数？

  
  InvalidateRect(main_hwnd, &uprect, 0);

  windowbsp.GameMsgLoop();// .WindowMsgLoop();
  windowbsp.DestoryBoard(hInstance, L"hello act");
  return 0;
}