#include "GdiTools.h"
#include <assert.h>


CGdiTools::CGdiTools()
{
}


CGdiTools::~CGdiTools()
{
}

int CGdiTools::BindBoard(void * board)
{
  assert(board);
  board_ = board;
  return 0;
}

int CGdiTools::Line(POINT start_point, POINT end_point)
{
  assert(board_);
  int ret;
  if (MoveToEx((HDC)board_, start_point.x, start_point.y, NULL)) {
    ret = LineTo((HDC)board_, end_point.x, end_point.y);
  }
  return ret ? 0 : 1;
}

int CGdiTools::Point(POINT point)
{
  assert(board_);
  int ret;
  ret = SetPixel((HDC)board_, point.x, point.y, color_);
  return -1 == ret ? 1 : 0;
}

int CGdiTools::Rectangle(POINT left_top_point, POINT right_bottom_point)
{
  assert(board_);
  int ret;
  ret = ::Rectangle((HDC)board_, left_top_point.x, left_top_point.y, right_bottom_point.x, right_bottom_point.y);
  return ret ? 0 : 1;
}

int CGdiTools::ChangeColor(COLORREF color)
{
  color_ = color;
  return 0;
}
