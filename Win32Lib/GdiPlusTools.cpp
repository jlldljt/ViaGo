#include "GdiPlusTools.h"
#include <assert.h>

//gdi+
#include <atlimage.h>	//CImage�������ͷ�ļ�
#pragma comment(lib,"Msimg32.lib")		//���ʹ��TransparentBlt��������Ŀ��ļ�


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
  *	1.���AlphaFormat�ֶ�Ϊ0,Դλͼ�е���������ʹ��ͬ���ĳ���alphaֵ����SourceConstantAlpha�ֶ��е�ֵ����ֵʵ������0��255��������0��1������0��ʾ��ȫ͸����255��ʾ��ȫ��͸����Ŀ��������255-SourceConstantAlphaֵ��Ϊalphaֵ��

  2. ���AlphaFormat�ֶε�ֵ��AC_SRC_ALPHA��Դ�豸�����ÿ�����ر����и��Ե�alphaͨ��������������32-bpp�������豸�����ģ�����ѡ����32-bpp DDB��DIB�ε��ڴ��豸�����ġ���Щ����£�ÿ��Դ������4��8λͨ��:�졢�̡�����alpha��ÿ�����ص�alphaͨ����SourceConstantAlpha�ֶ�һ�����ڰ�Դ��Ŀ����������ʵ�����ڼ��������ʽ����:

  Tmp.Red = Src.Red * SourceConstantAlpha / 255;

  Tmp.Green = Src.Green * SourceConstantAlpha / 255;

  Tmp.Blue = Src.Blue * SourceConstantAlpha / 255;

  Tmp.Alpha = Src.Alpha * SourceConstantAlpha / 255;

  Beta = 255 �C Tmp.alpha;

  Dst.Red = Tmp.Red + Round((Beta * Dst.Red )/255);

  Dst.Green = Tmp.Green + Round((Beta * Dst.Green)/255);

  Dst.Blue = Tmp.Blue + Round((Beta * Dst.Blue )/255);

  Dst.Alpha = Tmp.Alpha + Round((Beta * Dst.Alpha)/255);
  */
  if (img->GetBPP() == 32) //ȷ�ϸ�ͼ�����Alphaͨ��
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

  if (bImg->GetBPP() == 32) //ȷ�ϸ�ͼ�����Alphaͨ��
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
    SRCAND);//����������
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
    SRCPAINT);//������ǰ��

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
    SRCAND);//����������
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
    SRCPAINT);//������ǰ��

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
//TODO��������ɫѡ��
int CGdiPlusTools::EraseBoard(RECT dest)
{
  FillRect((HDC)board_, &dest, (HBRUSH)GetStockObject(WHITE_BRUSH));
  return 0;
}
