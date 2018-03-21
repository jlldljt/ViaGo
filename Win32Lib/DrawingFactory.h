/*管理CDrawingBase*的对象，创建，删除，绘制不放这里
管理画布，并将画布刷新到窗口
*/

#pragma once

#include "DrawingBase.h"//元素
//std::list
#include<list>


class CDrawingFactory
{
public:
  CDrawingFactory();
  ~CDrawingFactory();
  enum CreateType {
    _BLANK_ = 0,
    _BUTTON_,
    _ANIMATION_BUTTON_,
  };
public:
  HWND hwnd_;//窗口
  RECT rect_;//窗口大小
  HDC hdc_;//窗口dc
  double times_x_, times_y_;
  //TODO:ADASDFFASDF
  CDrawingBoard *board_;//画板，控件都画到这里

  CDrawingBase* focus_base_;//当前焦点元素的id
  std::list<CDrawingBase*> drawings_;//通过Add IMAGE函数添加CDrawingBase元素，带*是模板多态
  std::list<CDrawingBase*>::iterator it_;
  //从后往前遍历，调用目标的鼠标消息处理函数。实现的事件都是鼠标的事件，故只要判断鼠标位置即可，第一个满足的元素既是所需要的
  //int
  //调用焦点控件的消息
  //绑定HWND，得到画布
  int Bind(void* object);
  int UnBind(void);
  //在画布上创建一个元素
  CDrawingBase* Create(RECT rect, CreateType type);
//private:
//  /*属性*/
//  CDrawingFactory factory_;//仓库
//  CDrawingCache cache_;//缓存
//                       /*方法*/
//
//public:
  LRESULT Event(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
  void Animation(DWORD tick);
//  //把仓库中的内容 绘制到 缓存中
//  int draw();
};

