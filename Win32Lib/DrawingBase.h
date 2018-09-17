
/*��ͼ���࣬��Ҫ��Ϊ����ͼԪ�صĻ��࣬��ȡͬһ��

ʹ��֮ǰ����bind

ȱ�ٶ�����֧�ֺ���
*/

#pragma once

#include <Windows.h>
#include <Windowsx.h>//GET_X_LPARAM


#include"HwndBoard.h"
#include"GdiPlusTools.h"
class CDrawingBase
{
public:
  CDrawingBase();
  CDrawingBase(void* object, RECT object_rect, RECT rect);
  ~CDrawingBase();
protected: // �����޷����ʣ���������Է���
  CDrawingBoard *board_;
  CDrawingTools *tools_;
public:  
  /*�¼��ṹ�壬��ʱ��Windows��Ԫ�أ��Ժ�����Լ�����һ��*/
  enum EnumEvent
  {
    PAINT,
    INVALID,
    LBUTTONDOWN,
    LBUTTONUP,
    MOUSEMOVE,
  };
  struct EventParam {
    UINT event;
    void* param1;
    void* param2;
  };
  //board��װ���������
  void EnableTransparent(bool enable = false);
  //tool��װ���������
  int AddImage(TCHAR* path);
  int ShowImage(int no);
  int ShowNextImage();
 // int GetShowedImageNo();
  //�������ص���Ϣ����ӿ�
  int EventBase(EventParam param);
  //�������صĻ��ƽӿ�
  int DrawingBase();
  //�������Զ�����Ϣ�Ĵ���ʽ�����ȹ�����ͨ���������������������
  virtual int Event(EventParam& param);
  //�������Զ�����Ʒ��������ȹ�����ͨ���������������������
  virtual int Drawing();
  //�������Զ��嶯������
  virtual void Animation(RECT *update_rect);
  //��һ��ϵͳ�ϻ�������������ֻ�ͼ��ʽ���ʻ���ʵ�ֵ�
  int Bind(void* object/*HWND*/,RECT object_rect, RECT rect);
  int Unbind();


};

