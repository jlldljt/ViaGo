//实现窗口的绑定和刷新
#pragma once
#include "DrawingBoard.h"
class CHwndBoard :
  public CDrawingBoard
{
public:
  CHwndBoard();
  CHwndBoard(void *object, RECT object_rect, RECT rect);
  ~CHwndBoard();

private:
  HDC hdc_;
  HDC buf_hdc_;
  HBITMAP hbmp_;
  
public:
  //创建画布
  int Create(void *object);
  int ChangeRect(RECT object_rect, RECT rect);
  //获取画布bufhdc
  int GetBoard(void** board);
  int GetObject(void** object);
  //销毁画布
  int Destory();
  //画到画布上
  int Draw();
  //刷新窗口区域
  int Paint(RECT update_rect);
  int Paint(void * object, RECT rect);
};

