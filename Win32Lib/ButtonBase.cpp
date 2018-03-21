#include "ButtonBase.h"

#include <assert.h>


CButtonBase::CButtonBase()
{
  Drawing();//��ʱ���������
}

CButtonBase::CButtonBase(void * object, RECT object_rect, RECT rect):CDrawingBase(object, object_rect, rect)
{
  //CDrawingBase::Bind(object, object_rect, rect);
  //param init
  move = { 0,0,0 };

  //init things
  Drawing();
  board_->Paint(rect);//Ϊ��һ��ʼ��ʱ�����ʾ����
}


CButtonBase::~CButtonBase()
{
}
//����TRUE�����಻�ٱ���
int CButtonBase::Event(EventParam& param)
{
  
  switch (param.event)						//switch��俪ʼ
  {

  case LBUTTONDOWN:
  {
    POINT pt = *(POINT*)(param.param1);
    if (board_->IsInRect(pt)) {
      RECT rect = board_->GetRect();
      move.down = 1;
      move.xPos = rect.left - pt.x;
      move.yPos = rect.top - pt.y;
      return TRUE;
    }
  }
  break;
  case LBUTTONUP:
  {
    move.down = 0;
    break;
  }
  case MOUSEMOVE:
    if (move.down) {
      POINT pt = *(POINT*)(param.param1);
      int x = pt.x + move.xPos;
      int y = pt.y + move.yPos;
      RECT* rect = (RECT*)(param.param2);//ָ�򷵻ص�rect��ͨ������invalid
      RECT rect1 = board_->GetRect();
      //*(RECT*)(param.param2) = board_->GetRect();
      board_->MoveTo(x, y);
      RECT rect2 = board_->GetRect();
     // Drawing();//��ʱ���������

      UnionRect(rect, &rect1, &rect2);//�ϲ�ˢ������
      //����invalid
      param.event = INVALID;
      return TRUE;
    }

    break;
  //case PAINT:
  //  RECT rect = *((RECT*)(param.param2));
  //  
  //  board_->Paint(rect);
  //  break;
  }
  
  return CDrawingBase::Event(param);
}

//û��������Զ�����ƺ�������ʾ�ã�����ʹ�õ�ʱ����Ҫ������buttonbase��������drawingʵ���Լ��ķ���
int CButtonBase::Drawing()
{
  assert(tools_ && board_);
  RECT rect = board_->GetRect();
  POINT left_top_point = { 0,0 };// rect.left, rect.top};
  POINT right_bottom_point = { rect.right - rect.left, rect.bottom - rect.top};

  //ֻ����left_top_point - right_bottom_point��Χ�ڻ�������hbmp����Ч

  //SelectObject((HDC)tools_->board_, GetStockObject(WHITE_BRUSH));
  tools_->Rectangle(left_top_point, right_bottom_point);
  tools_->Line(left_top_point, right_bottom_point);
  return 0;
}

void CButtonBase::Animation(RECT *update_rect)
{
  //board_->Move(1, y);
}
