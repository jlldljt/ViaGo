#include "DrawingBsp.h"



CDrawingBsp::CDrawingBsp()
{
}


CDrawingBsp::~CDrawingBsp()
{
}

int CDrawingCache::Create(HWND hWnd)
{
  assert(NULL != hWnd);
  GetClientRect(hWnd, &rect_);
  hdc_ = GetDC(hWnd);
  buf_hdc_ = CreateCompatibleDC(hdc_);  //创建一个和hdc兼容的内存DC
  hbmp_ = CreateCompatibleBitmap(hdc_, rect_.right - rect_.left, rect_.bottom - rect_.top);
  SelectObject(buf_hdc_, hbmp_);
  hwnd_ = hWnd;
  return 0;
}
int CDrawingCache::Destory()
{
  DeleteObject(hbmp_);
  DeleteDC(buf_hdc_);
  ReleaseDC(hwnd_, hdc_);
  return 0;
}
//0失败
int CDrawingCache::Paint(RECT update_rect)
{
  return BitBlt(hdc_,
    update_rect.left,
    update_rect.top,
    update_rect.right - update_rect.left,
    update_rect.bottom - update_rect.top,
    buf_hdc_,
    update_rect.left,
    update_rect.top,
    SRCCOPY);
}
