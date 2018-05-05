#pragma once
#include "DrawingTools.h"
//CImage类包含的头文件
#include <atlimage.h>	
//添加使用TransparentBlt函数所需的库文件
#pragma  comment(lib,"Msimg32.lib")
#include <vector>

class CGdiPlusTools :
  public CDrawingTools
{
public:
  CGdiPlusTools();
  CGdiPlusTools(void *board);
  ~CGdiPlusTools();

public:
  //绑定画板。成功0，失败1
  int BindBoard(void *board);
  int UnBind();
  //画直线。成功0，失败1
  int Line(POINT start_point, POINT end_point);
  //画一像素点。成功0，失败1
  int Point(POINT point);
  //画一矩形。成功0，失败1
  int Rectangle(POINT left_top_point, POINT right_bottom_point);
  //改变线条颜色。成功0，失败1
  int ChangeColor(COLORREF color);

  //img相关操作
private:
  int img_num_;//图数量
  int current_img_;//当前图
  std::vector<CImage*> imgs_;//通过Add IMAGE函数添加CDrawingBase元素，带*是模板多态
  std::vector<CImage*> imgs_bg_;
  std::vector<CImage*>::iterator it_;
public:
  int AddImage(TCHAR* path);
  int StretchBlt(RECT dest, RECT src, int no);
  int StretchBlt(RECT dest, int no);
};

