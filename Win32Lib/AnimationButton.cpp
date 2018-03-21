#include "AnimationButton.h"



CAnimationButton::CAnimationButton()
{
  
}

CAnimationButton::CAnimationButton(void * object, RECT object_rect, RECT rect)
  :CButtonBase(object, object_rect, rect)
{
  //Bind(object, object_rect, rect);
  x_ = 1;
  y_ = 1;
}

CAnimationButton::~CAnimationButton()
{
}

void CAnimationButton::Animation(RECT *update_rect)
{
  RECT rect1 = board_->GetRect();
  int ret = board_->Move(x_, y_);
  RECT rect2 = board_->GetRect();
  UnionRect(update_rect, &rect1, &rect2);//�ϲ�ˢ������
  //board_->Paint(*update_rect);//��paint�¼�һ���ʱ��Ӧ�ý���paint�����ƣ�����������Ϸ

  if (ret & LEFT_FLAG) x_ = 1;
  else if (ret & RIGHT_FLAG) x_ = -1;

  if (ret & TOP_FLAG) y_ = 1;
  else if (ret & BOTTOM_FLAG) y_ = -1;
}
