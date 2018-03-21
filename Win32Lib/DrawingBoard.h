//创建绘制画板，比如hwnd 的 hdc，或者 image的data等
//针对绘制区域的部分操作放在这里实现
#pragma once
#include <Windows.h>

#define LEFT_FLAG 0x01
#define TOP_FLAG 0x02
#define RIGHT_FLAG 0x04
#define BOTTOM_FLAG 0x08

class CDrawingBoard
{
public:
  CDrawingBoard();
  ~CDrawingBoard() {};


  RECT rect_;
  RECT master_rect_;
  bool transparent_;

  virtual void EnableTransparent(bool enable);

  //创建画布
  virtual int Create(void *object) = 0;
  virtual int ChangeRect(RECT object_rect, RECT rect) = 0;
  //获取画布
  virtual int GetBoard(void** board) = 0;
  //获取画布
  virtual int GetObject(void** object) = 0;
  //销毁画布
  virtual int Destory() = 0;
  //画到画布上
  virtual int Draw() = 0;
  //刷新窗口区域
  virtual int Paint(RECT update_rect) = 0;
  virtual int Paint(void * object, RECT rect) = 0;//带缩放
  //移动
  virtual int Move(int& offset_x, int& offset_y);
  virtual int MoveTo(int & x, int & y);
  //判断是否在窗口区域
  virtual bool IsInRect(POINT point);

  //得到本board的区域
  virtual RECT GetRect();
  //得到父board的区域
  virtual RECT GetMasterRect();
};

