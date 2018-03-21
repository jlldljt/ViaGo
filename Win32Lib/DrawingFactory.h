/*����CDrawingBase*�Ķ��󣬴�����ɾ�������Ʋ�������
����������������ˢ�µ�����
*/

#pragma once

#include "DrawingBase.h"//Ԫ��
//std::list
#include<list>


class CDrawingFactory
{
public:
  CDrawingFactory();
  ~CDrawingFactory();
  enum CreateType {
    _BLANK_ = 0,
    _BUTTON_,
    _ANIMATION_BUTTON_,
  };
public:
  HWND hwnd_;//����
  RECT rect_;//���ڴ�С
  HDC hdc_;//����dc
  double times_x_, times_y_;
  //TODO:ADASDFFASDF
  CDrawingBoard *board_;//���壬�ؼ�����������

  CDrawingBase* focus_base_;//��ǰ����Ԫ�ص�id
  std::list<CDrawingBase*> drawings_;//ͨ��Add IMAGE�������CDrawingBaseԪ�أ���*��ģ���̬
  std::list<CDrawingBase*>::iterator it_;
  //�Ӻ���ǰ����������Ŀ��������Ϣ��������ʵ�ֵ��¼����������¼�����ֻҪ�ж����λ�ü��ɣ���һ�������Ԫ�ؼ�������Ҫ��
  //int
  //���ý���ؼ�����Ϣ
  //��HWND���õ�����
  int Bind(void* object);
  int UnBind(void);
  //�ڻ����ϴ���һ��Ԫ��
  CDrawingBase* Create(RECT rect, CreateType type);
//private:
//  /*����*/
//  CDrawingFactory factory_;//�ֿ�
//  CDrawingCache cache_;//����
//                       /*����*/
//
//public:
  LRESULT Event(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
  void Animation(DWORD tick);
//  //�Ѳֿ��е����� ���Ƶ� ������
//  int draw();
};

