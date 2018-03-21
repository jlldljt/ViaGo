//�������ƻ��壬����hwnd �� hdc������ image��data��
//��Ի�������Ĳ��ֲ�����������ʵ��
#pragma once
#include <Windows.h>

#define LEFT_FLAG 0x01
#define TOP_FLAG 0x02
#define RIGHT_FLAG 0x04
#define BOTTOM_FLAG 0x08

class CDrawingBoard
{
public:
  CDrawingBoard();
  ~CDrawingBoard() {};


  RECT rect_;
  RECT master_rect_;
  bool transparent_;

  virtual void EnableTransparent(bool enable);

  //��������
  virtual int Create(void *object) = 0;
  virtual int ChangeRect(RECT object_rect, RECT rect) = 0;
  //��ȡ����
  virtual int GetBoard(void** board) = 0;
  //��ȡ����
  virtual int GetObject(void** object) = 0;
  //���ٻ���
  virtual int Destory() = 0;
  //����������
  virtual int Draw() = 0;
  //ˢ�´�������
  virtual int Paint(RECT update_rect) = 0;
  virtual int Paint(void * object, RECT rect) = 0;//������
  //�ƶ�
  virtual int Move(int& offset_x, int& offset_y);
  virtual int MoveTo(int & x, int & y);
  //�ж��Ƿ��ڴ�������
  virtual bool IsInRect(POINT point);

  //�õ���board������
  virtual RECT GetRect();
  //�õ���board������
  virtual RECT GetMasterRect();
};

