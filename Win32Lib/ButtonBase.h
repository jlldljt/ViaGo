#pragma once
#include "DrawingBase.h"
class CButtonBase :
  public CDrawingBase
{
private:
  struct  tagMove
  {
    bool down;
    int xPos;
    int yPos;
  };
  tagMove move;
public:
  CButtonBase();
  CButtonBase(void* object, RECT object_rect, RECT rect);
  ~CButtonBase();

  virtual int Event(EventParam& param);//派生类自定义消息的处理方式，调度工厂类通过基类调用派生类来处理
  virtual int Drawing();//派生类自定义绘制方法，调度工厂类通过基类调用派生类来处理
  virtual void Animation(RECT *update_rect);
};

