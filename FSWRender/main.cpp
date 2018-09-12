/*
 * -----------------------------------------------------------------------
 *  Name:   main.cpp
 *  Desc:   本文件包含了程序的入口函数。
 *  Author: Yish
 *  Date:   2011/7/22
 *  ---------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * -----------------------------------------------------------------------
 */


#include "WinApp.h"


int WINAPI WinMain( HINSTANCE hInst,HINSTANCE,LPSTR,int )
{
    WinApp app;
    return app.Init(hInst) ? app.Run() : 0;
}