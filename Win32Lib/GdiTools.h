#pragma once
#include "DrawingTools.h"
class CGdiTools : 
  public CDrawingTools
{
public:
  CGdiTools();
  ~CGdiTools();

public:
  int BindBoard(void *board);
  int Line(POINT start_point, POINT end_point);
  int Point(POINT point);
  int Rectangle(POINT left_top_point, POINT right_bottom_point);
  int ChangeColor(COLORREF color);
};

