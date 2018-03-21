/*专门用于绘制，管理元素和画布，并能够将factory中的元素绘制到cache中*/
/*准备废弃中*/
#pragma once

#include "DrawingFactory.h"
#include "DrawingBsp.h"


class CDrawing
{
public:
  CDrawing();
  ~CDrawing();
private:
  /*属性*/
  CDrawingFactory factory_;//仓库
  CDrawingCache cache_;//缓存
  /*方法*/

public:
  int Bind(HWND hwnd);
  int Unbind();
  //把仓库中的内容 绘制到 缓存中
  int draw();
};

