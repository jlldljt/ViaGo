
/*绘图基类，主要作为后续图元素的基类，提取同一性

使用之前需先bind

缺少动画的支持函数
*/

#pragma once

#include <Windows.h>
#include <Windowsx.h>//GET_X_LPARAM


#include"HwndBoard.h"
#include"GdiPlusTools.h"
class CDrawingBase
{
public:
  CDrawingBase();
  CDrawingBase(void* object, RECT object_rect, RECT rect);
  ~CDrawingBase();
protected: // 类外无法访问，派生类可以访问
  CDrawingBoard *board_;
  CDrawingTools *tools_;
public:  
  /*事件结构体，暂时用Windows的元素，以后可以自己定义一个*/
  enum EnumEvent
  {
    PAINT,
    INVALID,
    LBUTTONDOWN,
    LBUTTONUP,
    MOUSEMOVE,
  };
  struct EventParam {
    UINT event;
    void* param1;
    void* param2;
  };
  //board封装，方便调用
  void EnableTransparent(bool enable = false);
  //tool封装，方便调用
  int AddImage(TCHAR* path);
  int ShowImage(int no);
  int ShowNextImage();
 // int GetShowedImageNo();
  //用于重载的消息处理接口
  int EventBase(EventParam param);
  //用于重载的绘制接口
  int DrawingBase();
  //派生类自定义消息的处理方式，调度工厂类通过基类调用派生类来处理
  virtual int Event(EventParam& param);
  //派生类自定义绘制方法，调度工厂类通过基类调用派生类来处理
  virtual int Drawing();
  //派生类自定义动画方法
  virtual void Animation(RECT *update_rect);
  //在一个系统上基本不会存在两种绘图方式，故基类实现掉
  int Bind(void* object/*HWND*/,RECT object_rect, RECT rect);
  int Unbind();


};

