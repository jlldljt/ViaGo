/*
 *DLLLib.h
 *��򵥵�DLL��û�������ԣ�ֻ�ǵ����˱���������������û��def�����Թ��Ʋ��ܿ繤��
 */
#ifndef _DLL_LIB_
#define _DLL_LIB_
#ifdef DLLLIBAPI

#else

#define DLLLIBAPI extern "C" __declspec(dllimport) //����extern "C"��˵������P532

#endif

/*exported variables*/
DLLLIBAPI int g_exportVariables;

/*exported functions*/
DLLLIBAPI int DoSomeThing(int arg1 , int arg2);

#endif