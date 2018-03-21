#include "ButtonBase.h"

#include <assert.h>


CButtonBase::CButtonBase()
{
  Drawing();//暂时在这里调用
}

CButtonBase::CButtonBase(void * object, RECT object_rect, RECT rect):CDrawingBase(object, object_rect, rect)
{
  //CDrawingBase::Bind(object, object_rect, rect);
  //param init
  move = { 0,0,0 };

  //init things
  Drawing();
  board_->Paint(rect);//为了一开始的时候就显示出来
}


CButtonBase::~CButtonBase()
{
}
//返回TRUE，父类不再遍历
int CButtonBase::Event(EventParam& param)
{
  
  switch (param.event)						//switch语句开始
  {

  case LBUTTONDOWN:
  {
    POINT pt = *(POINT*)(param.param1);
    if (board_->IsInRect(pt)) {
      RECT rect = board_->GetRect();
      move.down = 1;
      move.xPos = rect.left - pt.x;
      move.yPos = rect.top - pt.y;
      return TRUE;
    }
  }
  break;
  case LBUTTONUP:
  {
    move.down = 0;
    break;
  }
  case MOUSEMOVE:
    if (move.down) {
      POINT pt = *(POINT*)(param.param1);
      int x = pt.x + move.xPos;
      int y = pt.y + move.yPos;
      RECT* rect = (RECT*)(param.param2);//指向返回的rect，通常用于invalid
      RECT rect1 = board_->GetRect();
      //*(RECT*)(param.param2) = board_->GetRect();
      board_->MoveTo(x, y);
      RECT rect2 = board_->GetRect();
     // Drawing();//暂时在这里调用

      UnionRect(rect, &rect1, &rect2);//合并刷新区域
      //返回invalid
      param.event = INVALID;
      return TRUE;
    }

    break;
  //case PAINT:
  //  RECT rect = *((RECT*)(param.param2));
  //  
  //  board_->Paint(rect);
  //  break;
  }
  
  return CDrawingBase::Event(param);
}

//没有意义的自定义绘制函数，演示用，我们使用的时候需要。派生buttonbase，并重载drawing实现自己的方法
int CButtonBase::Drawing()
{
  assert(tools_ && board_);
  RECT rect = board_->GetRect();
  POINT left_top_point = { 0,0 };// rect.left, rect.top};
  POINT right_bottom_point = { rect.right - rect.left, rect.bottom - rect.top};

  //只能在left_top_point - right_bottom_point范围内画，出了hbmp，无效

  //SelectObject((HDC)tools_->board_, GetStockObject(WHITE_BRUSH));
  tools_->Rectangle(left_top_point, right_bottom_point);
  tools_->Line(left_top_point, right_bottom_point);
  return 0;
}

void CButtonBase::Animation(RECT *update_rect)
{
  //board_->Move(1, y);
}
