/*׼�������У���drawing board*/

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
˫������ͼ��
����windows��api����������
���ǵ�drawing�ǻ������cache��
���ڵ�onpaint��������ṩһ������������ˢ��������ȥ
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
  
  //��������
  int Create(HWND hWnd);
  int Destory();
  //����������
  int Draw();
  //ˢ�´���
  int Paint(RECT update_rect);
};
