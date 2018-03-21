/*
 *DLLLib.h
 *最简单的DLL，没有特殊性，只是导出了变量，函数，甚至没用def，所以估计不能跨工具
 */
#ifndef _DLL_LIB_
#define _DLL_LIB_
#ifdef DLLLIBAPI

#else

#define DLLLIBAPI extern "C" __declspec(dllimport) //关于extern "C"的说明，见P532

#endif

/*exported variables*/
DLLLIBAPI int g_exportVariables;

/*exported functions*/
DLLLIBAPI int DoSomeThing(int arg1 , int arg2);

#endif