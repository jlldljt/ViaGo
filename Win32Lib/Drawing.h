/*ר�����ڻ��ƣ�����Ԫ�غͻ��������ܹ���factory�е�Ԫ�ػ��Ƶ�cache��*/
/*׼��������*/
#pragma once

#include "DrawingFactory.h"
#include "DrawingBsp.h"


class CDrawing
{
public:
  CDrawing();
  ~CDrawing();
private:
  /*����*/
  CDrawingFactory factory_;//�ֿ�
  CDrawingCache cache_;//����
  /*����*/

public:
  int Bind(HWND hwnd);
  int Unbind();
  //�Ѳֿ��е����� ���Ƶ� ������
  int draw();
};

