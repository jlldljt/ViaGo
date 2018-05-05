#pragma once
#include "DrawingTools.h"
//CImage�������ͷ�ļ�
#include <atlimage.h>	
//���ʹ��TransparentBlt��������Ŀ��ļ�
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
  //�󶨻��塣�ɹ�0��ʧ��1
  int BindBoard(void *board);
  int UnBind();
  //��ֱ�ߡ��ɹ�0��ʧ��1
  int Line(POINT start_point, POINT end_point);
  //��һ���ص㡣�ɹ�0��ʧ��1
  int Point(POINT point);
  //��һ���Ρ��ɹ�0��ʧ��1
  int Rectangle(POINT left_top_point, POINT right_bottom_point);
  //�ı�������ɫ���ɹ�0��ʧ��1
  int ChangeColor(COLORREF color);

  //img��ز���
private:
  int img_num_;//ͼ����
  int current_img_;//��ǰͼ
  std::vector<CImage*> imgs_;//ͨ��Add IMAGE�������CDrawingBaseԪ�أ���*��ģ���̬
  std::vector<CImage*> imgs_bg_;
  std::vector<CImage*>::iterator it_;
public:
  int AddImage(TCHAR* path);
  int StretchBlt(RECT dest, RECT src, int no);
  int StretchBlt(RECT dest, int no);
};

