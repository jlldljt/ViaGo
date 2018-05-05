/*
                 windows api窗口的基础类
                 包括Windows窗体（子窗体）的创建，注意子窗体不支持ID，需客户端自己保持HWND
                 消息循环，二个，一个用于窗口，一个用于游戏
                 这部分由于与系统相关，不做深度封装处理

*/
#pragma once
#include <windowsx.h>//一些扩充定义，可以酌情增减
#include <Windows.h>//因为类定义中用到了windows的相关类型，故不能放到cpp中

//LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
typedef LRESULT(*pWNDPROC)(HWND, UINT, WPARAM, LPARAM);
typedef void(*pCARTOON)(DWORD);
struct CallBackFunction {
  pWNDPROC lbuttondown;
  pWNDPROC windows_message;
  pCARTOON cartoon;
};
class CWindowBsp;
union  tagProc //用于转换类成员方法指针到普通函数指针(地址相同)
{

  LRESULT (CALLBACK *wnd_proc)(HWND, UINT, WPARAM, LPARAM);

  LRESULT (CALLBACK CWindowBsp::*in_class_wnd_proc)(HWND, UINT, WPARAM, LPARAM);

};

class CWindowBsp
{
public:
	CWindowBsp();
	~CWindowBsp();
	CWindowBsp(const CWindowBsp&) = delete;//禁止拷贝
	CWindowBsp& operator=(const CWindowBsp&) = delete;//禁止移动

private:


public:
	//HWND hwnd_;
  static CallBackFunction callback_;
  static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);//1027
  tagProc proc_;
private:
	
  
  HWND CreateWin32Window(HINSTANCE hInstance, LPCWSTR lpName, DWORD dwStyle, HWND hWndParent);
public:
	HWND CreateBoard(HINSTANCE hInstance, LPCWSTR lpName, RECT rect, HWND parentwnd);
  bool DestoryBoard(HINSTANCE hInstance, LPCWSTR lpName);
  //HWND get_hwnd();
  void Cartoon(DWORD tick);//能够控制控件的一些动画，必须在父窗口定时器或msg loop中调用，并传入间隔
  void WindowMsgLoop();
  void GameMsgLoop();
  bool ChangeIcon(HINSTANCE hInstance, HWND hWnd, LPCWSTR lpPath);
};

