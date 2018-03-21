/*包装绘图的工具类，比如包装gdi/gdi+，directx等不同的api接口*/
#pragma once
#include <Windows.h>
class CDrawingTools
{
public:
  CDrawingTools() {
    board_ = NULL;
    color_ = 0;
  };
  ~CDrawingTools() { board_ = NULL; };
  void *board_;
  COLORREF color_;
  virtual int BindBoard(void *board) = 0;
  virtual int UnBind() = 0;
  virtual int Line(POINT start_point, POINT end_point) = 0;
  virtual int Point(POINT point) = 0;
  virtual int Rectangle(POINT left_top_point, POINT right_bottom_point) = 0;
  virtual int ChangeColor(COLORREF color) = 0;

  //img相关操作
  virtual int AddImage(TCHAR* path) = 0;
  virtual int StretchBlt(RECT dest, RECT src, int no) = 0;
  virtual int StretchBlt(RECT dest, int no) = 0;
};

