#include "DrawingBase.h"

#include <assert.h>


CDrawingBase::CDrawingBase()
{
}

CDrawingBase::CDrawingBase(void * object, RECT object_rect, RECT rect)
{
  Bind(object, object_rect, rect);
  //board_->Paint(rect);//为了一开始的时候就显示出来
}


CDrawingBase::~CDrawingBase()
{
}

int CDrawingBase::AddImage(TCHAR * path)
{
  return tools_->AddImage(path);
}

int CDrawingBase::ShowImage(int no)
{
  return tools_->StretchBlt(board_->GetRect(), no);//TODO：未调试
}

int CDrawingBase::EventBase(EventParam param)
{
  return 0;
}

int CDrawingBase::DrawingBase()
{
  return 0;
}

int CDrawingBase::Event(EventParam& param)
{
  switch (param.event)						//switch语句开始
  {
  case PAINT:
    RECT rect = *((RECT*)(param.param2));

    board_->Paint(rect);
    break;
  }
  return FALSE;
}

int CDrawingBase::Drawing()
{
  return 0;
}

void CDrawingBase::Animation(RECT *update_rect)
{
}

int CDrawingBase::Bind(void* object, RECT object_rect, RECT rect)
{
  assert(!tools_ && !board_);

  board_ = new CHwndBoard(object, object_rect, rect);
  void * board = NULL;
  int ret = board_->GetBoard(&board);
  if (!ret) {
    tools_ = new CGdiPlusTools(board);
  }
    
  return 0;
}

int CDrawingBase::Unbind()
{
  tools_->UnBind();
  board_->Destory();
  return 0;
}