#include <Windowsx.h>
#include <Windows.h>//这两个东西竟然都要包含，不然_tWinMain都会报错,按p738的说法，应该在windows调用之后马上调用windowsx,但是貌似反过来也没有问题
#include <stdlib.h>
#include <tchar.h>
#include <strsafe.h>//必须放在tchar.h之后
#include"..\DLL1\DLLLib.h"
#ifdef _DEBUG
#pragma comment(lib,"..\\debug\\DLL1.lib")
#else
#pragma comment(lib,"..\\release\\DLL1.lib")
#endif
//int WINAPI _tWinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd ){
int WINAPI _tWinMain(HINSTANCE hinstExe, HINSTANCE, PTSTR pszCmdLine, int){
	int a = 10 , b = 20;

	TCHAR tch[100];
	StringCchPrintf(tch , _countof(tch) , TEXT("%d + %d = %d"),
		a ,b , DoSomeThing(a,b));
	MessageBox(NULL , tch , TEXT("add") , MB_OK);

	StringCchPrintf(tch , _countof(tch) , TEXT("END:%d") ,g_exportVariables );
	MessageBox(NULL , tch , TEXT("RESULT") , MB_OK);
	return(0);

}