/*
                 windows api���ڵĻ�����
                 ����Windows���壨�Ӵ��壩�Ĵ�����ע���Ӵ��岻֧��ID����ͻ����Լ�����HWND
                 ��Ϣѭ����������һ�����ڴ��ڣ�һ��������Ϸ
                 �ⲿ��������ϵͳ��أ�������ȷ�װ����

*/
#pragma once
#include <windowsx.h>//һЩ���䶨�壬������������
#include <Windows.h>//��Ϊ�ඨ�����õ���windows��������ͣ��ʲ��ܷŵ�cpp��

//LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
typedef LRESULT(*pWNDPROC)(HWND, UINT, WPARAM, LPARAM);
typedef void(*pCARTOON)(DWORD);
struct CallBackFunction {
  pWNDPROC lbuttondown;
  pWNDPROC windows_message;
  pCARTOON cartoon;
};
class CWindowBsp;
union  tagProc //����ת�����Ա����ָ�뵽��ͨ����ָ��(��ַ��ͬ)
{

  LRESULT (CALLBACK *wnd_proc)(HWND, UINT, WPARAM, LPARAM);

  LRESULT (CALLBACK CWindowBsp::*in_class_wnd_proc)(HWND, UINT, WPARAM, LPARAM);

};

class CWindowBsp
{
public:
	CWindowBsp();
	~CWindowBsp();
	CWindowBsp(const CWindowBsp&) = delete;//��ֹ����
	CWindowBsp& operator=(const CWindowBsp&) = delete;//��ֹ�ƶ�

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
  void Cartoon(DWORD tick);//�ܹ����ƿؼ���һЩ�����������ڸ����ڶ�ʱ����msg loop�е��ã���������
  void WindowMsgLoop();
  void GameMsgLoop();
  bool ChangeIcon(HINSTANCE hInstance, HWND hWnd, LPCWSTR lpPath);
};

