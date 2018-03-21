/*准备废弃中，见drawing board*/

#pragma once
#include<Windows.h>
#include <assert.h>
class CDrawingBsp
{
public:
  CDrawingBsp();
  ~CDrawingBsp();
};

/*
双缓冲贴图类
利用windows的api来创建画布
我们的drawing是画到这个cache上
窗口的onpaint由这个类提供一个函数来根据刷新区域画上去
*/
class CDrawingCache {
public:
  HDC hdc_;
  HDC buf_hdc_;
  HBITMAP hbmp_;
  HWND hwnd_;
  RECT rect_;
  CDrawingCache() {}
  ~CDrawingCache() {}
  
  //创建画布
  int Create(HWND hWnd);
  int Destory();
  //画到画布上
  int Draw();
  //刷新窗口
  int Paint(RECT update_rect);
};
