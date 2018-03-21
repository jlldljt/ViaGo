#pragma once
#include "ButtonBase.h"
class CAnimationButton :
  public CButtonBase
{
public:
  CAnimationButton();
  CAnimationButton(void * object, RECT object_rect, RECT rect);
  ~CAnimationButton();
  int x_;//¡Ÿ ±
  int y_;//¡Ÿ ±
  virtual void Animation(RECT *update_rect);
};

