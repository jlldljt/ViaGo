#include "DrawingFactory.h"
#include <assert.h>
//控件
#include "ButtonBase.h"
#include "AnimationButton.h"

CDrawingFactory::CDrawingFactory()
{
}


CDrawingFactory::~CDrawingFactory()
{
}

int CDrawingFactory::Bind(void * object)
{
  assert(!board_);
  hwnd_ = (HWND)object;
  GetClientRect(hwnd_, &rect_);
  hdc_ = GetDC(hwnd_);
  times_x_ = 1;
  times_y_ = 1;
  board_ = new CHwndBoard(hdc_, rect_, rect_);
  return 0;
}

int CDrawingFactory::UnBind(void)
{
  delete board_;
  ReleaseDC(hwnd_, hdc_);
  return 0;
}

CDrawingBase* CDrawingFactory::Create(RECT rect, CreateType type)
{
  CDrawingBase *base = NULL;
  void* object = NULL;
  switch (type)
  {
  case CDrawingFactory::_BUTTON_:
    //base = new CButtonBase();

    if (0 == board_->GetBoard(&object))
      base = new CButtonBase(object, rect_, rect);
      //base->Bind(object, rect_, rect);
    break;
  case CDrawingFactory::_BLANK_:
    if (0 == board_->GetBoard(&object))
      base = new CDrawingBase(object, rect_, rect);
    //base->Bind(object, rect_, rect);
    break;
  case CDrawingFactory::_ANIMATION_BUTTON_:
    if (0 == board_->GetBoard(&object))
      base = new CAnimationButton(object, rect_, rect);
    //base->Bind(object, rect_, rect);
    break;
  default:
    break;
  }
  
  drawings_.push_back(base);
  return base;
}

LRESULT CDrawingFactory::Event(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  LRESULT ret = FALSE;
  //消息中可能用到的参数
  POINT pt;
  RECT rect = {0,0,0,0};

  if (hwnd != hwnd_)
    return ret;

  CDrawingBase::EventParam param = { ~0, &pt, &rect };

  switch (message)						//switch语句开始，主要是填充param结构体
  {

  case WM_LBUTTONDOWN:
  {
    pt = { GET_X_LPARAM(lParam) , GET_Y_LPARAM(lParam) };
    param.event = CDrawingBase::EnumEvent::LBUTTONDOWN;
    //param.param1 = &pt;
    break;
  }

  case WM_LBUTTONUP:
  {
    pt = { GET_X_LPARAM(lParam) , GET_Y_LPARAM(lParam) };
    param.event = CDrawingBase::EnumEvent::LBUTTONUP;
    //param.param1 = &pt;
    break;
  }
  case WM_MOUSEMOVE:
    pt = { GET_X_LPARAM(lParam) , GET_Y_LPARAM(lParam) };
    param.event = CDrawingBase::EnumEvent::MOUSEMOVE;
    //param.param1 = &pt;
    break;
  case WM_PAINT:
    PAINTSTRUCT ps;
    BeginPaint(hwnd, &ps);
    rect = ps.rcPaint;
    EndPaint(hwnd, &ps);
    param.event = CDrawingBase::EnumEvent::PAINT;
    //param.param2 = &rect;
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
  }

  //调整（窗口缩放
  pt.x *= times_x_;
  pt.y *= times_y_;
  rect.top *= times_y_;
  rect.bottom *= times_y_;
  rect.left *= times_x_;
  rect.right *= times_x_;

  //控件的board都画到factory的board上
  //排在第一的
  
  CDrawingBase* base = NULL;
  

  if (param.event == CDrawingBase::EnumEvent::PAINT) {
    //factory的board绘制到窗口dc上，绘制顺序是排前的先画
    {
      //std::list<CDrawingBase*>::iterator it;
      for (std::list<CDrawingBase*>::iterator it = drawings_.begin(); it != drawings_.end(); ++it) {

        base = *it;
        if (ret = base->Event(param)) {
          //focus_base_ = base;
          break;
        }
      }
      //调整（窗口缩放
      RECT hwnd_rect;
      HDC hwnd_hdc;
      hwnd_hdc = GetDC(hwnd);
      GetClientRect(hwnd, &hwnd_rect);
      times_x_ = (double)(rect_.right) / (double)(hwnd_rect.right);
      times_y_ = (double)(rect_.bottom) / (double)(hwnd_rect.bottom);
      board_->Paint(hwnd_hdc, hwnd_rect);
      ReleaseDC(hwnd, hwnd_hdc);
    }
  } else {
    //事件响应顺序是排后的先响应
    for (std::list<CDrawingBase*>::reverse_iterator it = drawings_.rbegin(); it != drawings_.rend(); ++it) {

      base = *it;
      if (ret = base->Event(param)) {
        focus_base_ = base;
        break;
      }
    }

    if (param.event == CDrawingBase::EnumEvent::INVALID) {
      rect.top = rect.top / times_y_ - 1;//防止计数误差导致刷新区域变小而留下残影，扩大一个像素
      rect.bottom = rect.bottom / times_y_ + 1;
      rect.left = rect.left / times_x_ - 1;
      rect.right = rect.right / times_x_ + 1;
      InvalidateRect(hwnd, &rect, 0);
    }
  }
  return ret;
}

void CDrawingFactory::Animation(DWORD tick)
{
  //DWORD		tPre = 0, tNow = 0, tTim;
  //
  //    tNow = GetTickCount();   //获取当前系统时间
  //    tTim = tNow - tPre;
  //    if (tTim >= 60)        //当此次循环运行与上次绘图时间相差0.05秒时再进行重绘操作
  //    {
  //      //重绘
  //      tPre = GetTickCount();   //获取当前系统时间
  // 
  //    }
  RECT update_rect = { 0,0,0,0 };
  CDrawingBase* base = NULL;
  for (std::list<CDrawingBase*>::iterator it = drawings_.begin(); it != drawings_.end(); ++it) {
  //for (std::list<CDrawingBase*>::reverse_iterator it = drawings_.rbegin(); it != drawings_.rend(); ++it) {
    RECT temp_rect = { 0,0,0,0 };
    base = *it;
    base->Animation(&temp_rect);
    UnionRect(&update_rect, &update_rect, &temp_rect);//合并刷新区域
  }
  InvalidateRect(hwnd_, &update_rect, 0);
  //board_->Paint(hwnd_, rect_);//和paint事件一起的时候，应该交给paint来绘制，除非做纯游戏
}


