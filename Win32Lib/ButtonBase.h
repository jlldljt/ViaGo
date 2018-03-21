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

  virtual int Event(EventParam& param);//�������Զ�����Ϣ�Ĵ���ʽ�����ȹ�����ͨ���������������������
  virtual int Drawing();//�������Զ�����Ʒ��������ȹ�����ͨ���������������������
  virtual void Animation(RECT *update_rect);
};

