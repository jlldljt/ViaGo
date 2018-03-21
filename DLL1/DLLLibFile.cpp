/*
 *DLLLibFile.cpp
 */

#include <Windows.h>

#define DLLLIBAPI extern "C" __declspec(dllexport)

#include "DLLLib.h"

/*exported variables*/
int g_exportVariables;

/*exported functions*/
int DoSomeThing(int arg1 , int arg2)
{
	g_exportVariables = arg1 + arg2;
	return g_exportVariables;
}