#pragma once
#include "ButtonBase.h"
class CAnimationButton :
  public CButtonBase
{
public:
  CAnimationButton();
  CAnimationButton(void * object, RECT object_rect, RECT rect);
  ~CAnimationButton();
  int x_;//��ʱ
  int y_;//��ʱ
  virtual void Animation(RECT *update_rect);
};

