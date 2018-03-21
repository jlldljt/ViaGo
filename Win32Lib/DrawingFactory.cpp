#include "DrawingFactory.h"
#include <assert.h>
//�ؼ�
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
  //��Ϣ�п����õ��Ĳ���
  POINT pt;
  RECT rect = {0,0,0,0};

  if (hwnd != hwnd_)
    return ret;

  CDrawingBase::EventParam param = { ~0, &pt, &rect };

  switch (message)						//switch��俪ʼ����Ҫ�����param�ṹ��
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
  }

  //��������������
  pt.x *= times_x_;
  pt.y *= times_y_;
  rect.top *= times_y_;
  rect.bottom *= times_y_;
  rect.left *= times_x_;
  rect.right *= times_x_;

  //�ؼ���board������factory��board��
  //���ڵ�һ��
  
  CDrawingBase* base = NULL;
  

  if (param.event == CDrawingBase::EnumEvent::PAINT) {
    //factory��board���Ƶ�����dc�ϣ�����˳������ǰ���Ȼ�
    {
      //std::list<CDrawingBase*>::iterator it;
      for (std::list<CDrawingBase*>::iterator it = drawings_.begin(); it != drawings_.end(); ++it) {

        base = *it;
        if (ret = base->Event(param)) {
          //focus_base_ = base;
          break;
        }
      }
      //��������������
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
    //�¼���Ӧ˳�����ź������Ӧ
    for (std::list<CDrawingBase*>::reverse_iterator it = drawings_.rbegin(); it != drawings_.rend(); ++it) {

      base = *it;
      if (ret = base->Event(param)) {
        focus_base_ = base;
        break;
      }
    }

    if (param.event == CDrawingBase::EnumEvent::INVALID) {
      rect.top = rect.top / times_y_ - 1;//��ֹ��������ˢ�������С�����²�Ӱ������һ������
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
  //    tNow = GetTickCount();   //��ȡ��ǰϵͳʱ��
  //    tTim = tNow - tPre;
  //    if (tTim >= 60)        //���˴�ѭ���������ϴλ�ͼʱ�����0.05��ʱ�ٽ����ػ����
  //    {
  //      //�ػ�
  //      tPre = GetTickCount();   //��ȡ��ǰϵͳʱ��
  // 
  //    }
  RECT update_rect = { 0,0,0,0 };
  CDrawingBase* base = NULL;
  for (std::list<CDrawingBase*>::iterator it = drawings_.begin(); it != drawings_.end(); ++it) {
  //for (std::list<CDrawingBase*>::reverse_iterator it = drawings_.rbegin(); it != drawings_.rend(); ++it) {
    RECT temp_rect = { 0,0,0,0 };
    base = *it;
    base->Animation(&temp_rect);
    UnionRect(&update_rect, &update_rect, &temp_rect);//�ϲ�ˢ������
  }
  InvalidateRect(hwnd_, &update_rect, 0);
  //board_->Paint(hwnd_, rect_);//��paint�¼�һ���ʱ��Ӧ�ý���paint�����ƣ�����������Ϸ
}


