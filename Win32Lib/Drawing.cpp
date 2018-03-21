#include "Drawing.h"



CDrawing::CDrawing()
{
}


CDrawing::~CDrawing()
{
}

int CDrawing::Bind(HWND hwnd)
{
  cache_.Create(hwnd);
  return 0;
}

int CDrawing::Unbind()
{
  cache_.Destory();
  return 0;
}

int CDrawing::draw()
{

  return 0;
}
