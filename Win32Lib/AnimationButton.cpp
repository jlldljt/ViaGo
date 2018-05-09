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
//TODO：这是示例，不能移动
void CAnimationButton::Animation(RECT *update_rect)
{
  RECT rect1 = board_->GetRect();
  int ret = board_->Move(x_, y_);
  RECT rect2 = board_->GetRect();
  UnionRect(update_rect, &rect1, &rect2);//合并刷新区域
  //board_->Paint(*update_rect);//和paint事件一起的时候，应该交给paint来绘制，除非做纯游戏
  //RECT tmp_rect = { 0, 0, rect2.right - rect2.left, rect2.bottom - rect2.top };
  //tools_->StretchBlt(tmp_rect, )
  static unsigned int time = 0;
  if (time++ == 10) {
    ShowNextImage();
    time = 0;
  }
  if (ret & LEFT_FLAG) x_ = 1;
  else if (ret & RIGHT_FLAG) x_ = -1;

  if (ret & TOP_FLAG) y_ = 1;
  else if (ret & BOTTOM_FLAG) y_ = -1;
}
