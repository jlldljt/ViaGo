#include "DrawingBoard.h"



CDrawingBoard::CDrawingBoard()
{
  transparent_ = false;
}

//
//CDrawingBoard::~CDrawingBoard()
//{
//}
//非0越界
//0正常
//#define LEFT_FLAG 0x01
//#define TOP_FLAG 0x02
//#define RIGHT_FLAG 0x04
//#define BOTTOM_FLAG 0x08
int CDrawingBoard::Move(int& offset_x, int& offset_y)
{
  int ret = 0;
  if (rect_.left + offset_x < master_rect_.left) {
    offset_x = master_rect_.left - rect_.left;
    ret |= LEFT_FLAG;
  }
  else if (rect_.right + offset_x > master_rect_.right) {
    offset_x = master_rect_.right - rect_.right;
    ret |= RIGHT_FLAG;
  }

  if (rect_.top + offset_y < master_rect_.top) {
    offset_y = master_rect_.top - rect_.top;
    ret |= TOP_FLAG;
  }
  else if (rect_.bottom + offset_y > master_rect_.bottom) {
    offset_y = master_rect_.bottom - rect_.bottom;
    ret |= BOTTOM_FLAG;
  }

  OffsetRect(&rect_, offset_x, offset_y);
  return ret;
}

int CDrawingBoard::MoveTo(int& x, int& y)
{
  int width = rect_.right - rect_.left;
  int height = rect_.bottom - rect_.top;
  //查看是否越界 主窗口
  if (x < master_rect_.left)
    x = master_rect_.left;
  else if (x + width > master_rect_.right)
    x = master_rect_.right - width;
  if (y < master_rect_.top)
    y = master_rect_.top;
  else if (y + height > master_rect_.bottom)
    y = master_rect_.bottom - height;

  SetRect(&rect_, x, y, x + width, y + height);
  return 0;
}

bool CDrawingBoard::IsInRect(POINT point)
{
  return PtInRect(&rect_, point);
}

RECT CDrawingBoard::GetRect()
{
  return rect_;
}

RECT CDrawingBoard::GetMasterRect()
{
  return master_rect_;
}

void CDrawingBoard::EnableTransparent(bool enable)
{
  transparent_ = enable;
}
