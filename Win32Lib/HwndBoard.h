//ʵ�ִ��ڵİ󶨺�ˢ��
#pragma once
#include "DrawingBoard.h"
class CHwndBoard :
  public CDrawingBoard
{
public:
  CHwndBoard();
  CHwndBoard(void *object, RECT object_rect, RECT rect);
  ~CHwndBoard();

private:
  HDC hdc_;
  HDC buf_hdc_;
  HBITMAP hbmp_;
  
public:
  //��������
  int Create(void *object);
  int ChangeRect(RECT object_rect, RECT rect);
  //��ȡ����bufhdc
  int GetBoard(void** board);
  int GetObject(void** object);
  //���ٻ���
  int Destory();
  //����������
  int Draw();
  //ˢ�´�������
  int Paint(RECT update_rect);
  int Paint(void * object, RECT rect);
};

