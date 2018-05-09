#include "HwndBoard.h"


#include <assert.h>

CHwndBoard::CHwndBoard()
{
  hdc_ = NULL;
  buf_hdc_ = NULL;
  hbmp_ = NULL;
//  hwnd_ = NULL;
}
//HDC 绑定 bufHDC
CHwndBoard::CHwndBoard(void * object, RECT object_rect, RECT rect)
{
  hdc_ = (HDC)object;
  rect_ = rect;
  master_rect_ = object_rect;

  buf_hdc_ = CreateCompatibleDC(hdc_);  //创建一个和hdc兼容的内存DC
  hbmp_ = CreateCompatibleBitmap(hdc_, rect_.right - rect_.left, rect_.bottom - rect_.top);
  SelectObject(buf_hdc_, hbmp_);

  RECT rect_fill = { 0, 0, rect_.right - rect_.left, rect_.bottom - rect_.top };
  FillRect(buf_hdc_, &rect_fill, (HBRUSH)GetStockObject(WHITE_BRUSH));//(HBRUSH) (COLOR_WINDOW+1)
  //Create(object);
}


CHwndBoard::~CHwndBoard()
{
  DeleteObject(hbmp_);
  DeleteDC(buf_hdc_);
}

int CHwndBoard::Create(void * object)
{
 // hdc_ = (HDC)object;
 // assert(NULL != hdc_);
 //// if (GetClientRect(hWnd, &rect_)) {
 ////   hdc_ = GetDC(hWnd);
 //   buf_hdc_ = CreateCompatibleDC(hdc_);  //创建一个和hdc兼容的内存DC
 //   hbmp_ = CreateCompatibleBitmap(hdc_, rect_.right - rect_.left, rect_.bottom - rect_.top);
 //   SelectObject(buf_hdc_, hbmp_);
 //   hwnd_ = hWnd;
 //// }
 // 
  return 0;
}

int CHwndBoard::ChangeRect(RECT object_rect, RECT rect)
{
  assert(hbmp_);
  rect_ = rect;
  master_rect_ = object_rect;

  DeleteObject(hbmp_);
  hbmp_ = CreateCompatibleBitmap(hdc_, rect_.right - rect_.left, rect_.bottom - rect_.top);
  SelectObject(buf_hdc_, hbmp_);

  return 0;
}

int CHwndBoard::GetBoard(void ** board)
{
  if (buf_hdc_) {
    *board = buf_hdc_;
    return 0;
  }
  return -1;
}

int CHwndBoard::GetObject(void ** object)
{
  if (hdc_) {
    *object = hdc_;
    return 0;
  }
  return -1;
}

int CHwndBoard::Destory()
{
  DeleteObject(hbmp_);
  DeleteDC(buf_hdc_);
  //ReleaseDC(hwnd_, hdc_);
  return 0;
}

int CHwndBoard::Draw()
{
  return 0;
}

//
int CHwndBoard::Paint(RECT update_rect)
{
  RECT rect = update_rect;

  bool intersected = IntersectRect(&rect, &update_rect, &rect_);
  if (!intersected)
    return 0;

  int x = rect.left - rect_.left;
  int y = rect.top - rect_.top;
  int width = rect.right - rect.left;
  int height = rect.bottom - rect.top;
  
  int ret;
  if (!transparent_) {
    ret = BitBlt(hdc_,
      rect.left,
      rect.top,
      rect.right - rect.left,
      rect.bottom - rect.top,
      buf_hdc_,
      x,
      y,
      SRCCOPY);
  }
  else {
    ret = TransparentBlt(
      hdc_,
      rect.left,
      rect.top,
      rect.right - rect.left,
      rect.bottom - rect.top,
      buf_hdc_,
      x,
      y,
      width,
      height,
      RGB(255, 255, 255)
    );
  }
  return 0;
}

int CHwndBoard::Paint(void * object, RECT rect)
{
  assert(object);
  HDC hdc = (HDC)object;

  SetStretchBltMode(hdc, HALFTONE);
  SetBrushOrgEx(hdc, 0, 0, NULL);

  StretchBlt(hdc, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top
    , buf_hdc_, rect_.left, rect_.top, rect_.right - rect_.left, rect_.bottom - rect_.top, /*SRCPAINT*/SRCCOPY);

  return 0;
}