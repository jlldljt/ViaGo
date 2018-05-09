#include "GdiPlusTools.h"
#include <assert.h>

//gdi+
#include <atlimage.h>	//CImage类包含的头文件
#pragma comment(lib,"Msimg32.lib")		//添加使用TransparentBlt函数所需的库文件


CGdiPlusTools::CGdiPlusTools()
{
  img_num_ = 0;
  blted_img_ = 0;
}

CGdiPlusTools::CGdiPlusTools(void * board)
{
  BindBoard(board);
}


CGdiPlusTools::~CGdiPlusTools()
{
  CImage* img;
    while (!imgs_.empty())
    {
      img = imgs_.back();
      imgs_.pop_back();
    }
    while (!imgs_bg_.empty())
    {
      img = imgs_bg_.back();
      imgs_bg_.pop_back();
    }
}

int CGdiPlusTools::BindBoard(void * board)
{
  assert(board);
  board_ = board;
  return 0;
}

int CGdiPlusTools::UnBind()
{
  board_ = NULL;
  return 0;
}

int CGdiPlusTools::Line(POINT start_point, POINT end_point)
{
  assert(board_);
  int ret;
  if (MoveToEx((HDC)board_, start_point.x, start_point.y, NULL)) {
    ret = LineTo((HDC)board_, end_point.x, end_point.y);
  }
  return ret ? 0 : 1;
}

int CGdiPlusTools::Point(POINT point)
{
  assert(board_);
  int ret;
  ret = SetPixel((HDC)board_, point.x, point.y, color_);
  return -1 == ret ? 1 : 0;
}

int CGdiPlusTools::Rectangle(POINT left_top_point, POINT right_bottom_point)
{
  assert(board_);
  int ret;
  HBRUSH newBrush = (HBRUSH)GetStockObject(/*WHITE_BRUSH*//*BLACK_BRUSH*/NULL_BRUSH);
  HBRUSH oldBrush = (HBRUSH)SelectObject((HDC)board_, (HBRUSH)newBrush);
  ret = ::Rectangle((HDC)board_, left_top_point.x, left_top_point.y, right_bottom_point.x, right_bottom_point.y);
  SelectObject((HDC)board_, (HBRUSH)oldBrush);
  DeleteObject((HBRUSH)newBrush);
  return ret ? 0 : 1;
}

int CGdiPlusTools::ChangeColor(COLORREF color)
{
  color_ = color;
  return 0;
}

int CGdiPlusTools::AddImage(TCHAR * path)
{
  CImage* img = new CImage();
  CImage* bImg = new CImage();
  img->Load(path);
  bImg->Load(path);
  if (img->IsNull() || bImg->IsNull())
  {
    delete img;
    img = NULL;
    delete bImg;
    bImg = NULL;
    return -1;
  }
  /*
  *	1.如果AlphaFormat字段为0,源位图中的所有像素使用同样的常量alpha值，即SourceConstantAlpha字段中的值，该值实际上是0和255，而不是0和1。这里0表示完全透明，255表示完全不透明。目标像素以255-SourceConstantAlpha值作为alpha值。

  2. 如果AlphaFormat字段的值是AC_SRC_ALPHA，源设备表面的每个像素必须有各自的alpha通道。即，必须是32-bpp的物理设备上下文，或是选中了32-bpp DDB和DIB段的内存设备上下文。这些情况下，每个源像素有4个8位通道:红、绿、蓝和alpha。每个像素的alpha通道和SourceConstantAlpha字段一起用于把源和目标混合起来。实际用于计算的运算式如下:

  Tmp.Red = Src.Red * SourceConstantAlpha / 255;

  Tmp.Green = Src.Green * SourceConstantAlpha / 255;

  Tmp.Blue = Src.Blue * SourceConstantAlpha / 255;

  Tmp.Alpha = Src.Alpha * SourceConstantAlpha / 255;

  Beta = 255 – Tmp.alpha;

  Dst.Red = Tmp.Red + Round((Beta * Dst.Red )/255);

  Dst.Green = Tmp.Green + Round((Beta * Dst.Green)/255);

  Dst.Blue = Tmp.Blue + Round((Beta * Dst.Blue )/255);

  Dst.Alpha = Tmp.Alpha + Round((Beta * Dst.Alpha)/255);
  */
  if (img->GetBPP() == 32) //确认该图像包含Alpha通道
  {
    int i;
    int j;
    for (i = 0; i<img->GetWidth(); i++)
    {
      for (j = 0; j<img->GetHeight(); j++)
      {
        byte *pByte = (byte *)img->GetPixelAddress(i, j);
        pByte[0] = pByte[0] * pByte[3] / 255;
        pByte[1] = pByte[1] * pByte[3] / 255;
        pByte[2] = pByte[2] * pByte[3] / 255;
      }
    }
  }

  if (bImg->GetBPP() == 32) //确认该图像包含Alpha通道
  {
    int i;
    int j;
    for (i = 0; i<bImg->GetWidth(); i++)
    {
      for (j = 0; j<bImg->GetHeight(); j++)
      {
        byte *pByte = (byte *)bImg->GetPixelAddress(i, j);
        pByte[0] = 255 - pByte[3];
        pByte[1] = 255 - pByte[3];
        pByte[2] = 255 - pByte[3];
      }
    }
  }
  imgs_.push_back(img);
  imgs_bg_.push_back(bImg);
  img_num_++;
  return 0;
}

int CGdiPlusTools::StretchBlt(RECT dest, RECT src,int no)
{
  //SetStretchBltMode((HDC)board_, HALFTONE);
  //SetBrushOrgEx((HDC)board_, 0, 0, NULL);

  imgs_bg_[no]->StretchBlt(
    (HDC)board_ , 
    dest.left, 
    dest.top, 
    dest.right-dest.left, 
    dest.bottom-dest.top, 
    src.left,
    src.top,
    src.right - src.left,
    src.bottom - src.top,
    SRCAND);//贴动作背景
  imgs_[no]->StretchBlt(
    (HDC)board_,
    dest.left,
    dest.top,
    dest.right - dest.left,
    dest.bottom - dest.top,
    src.left,
    src.top,
    src.right - src.left,
    src.bottom - src.top, 
    SRCPAINT);//贴动作前景

  blted_img_ = no;

  return 0;
}

int CGdiPlusTools::StretchBlt(RECT dest, int no)
{
  imgs_bg_[no]->StretchBlt(
    (HDC)board_,
    dest.left,
    dest.top,
    dest.right - dest.left,
    dest.bottom - dest.top,
    0,
    0,
    imgs_bg_[no]->GetWidth(),
    imgs_bg_[no]->GetHeight(),
    SRCAND);//贴动作背景
  imgs_[no]->StretchBlt(
    (HDC)board_,
    dest.left,
    dest.top,
    dest.right - dest.left,
    dest.bottom - dest.top,
    0,
    0,
    imgs_[no]->GetWidth(),
    imgs_[no]->GetHeight(),
    SRCPAINT);//贴动作前景

  blted_img_ = no;

  return 0;
}

int CGdiPlusTools::GetBltedImageNo()
{
  return blted_img_;
}

int CGdiPlusTools::GetImageNum()
{
  return img_num_;
}
//TODO：增加颜色选择
int CGdiPlusTools::EraseBoard(RECT dest)
{
  FillRect((HDC)board_, &dest, (HBRUSH)GetStockObject(WHITE_BRUSH));
  return 0;
}
