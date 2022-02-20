#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // 从 Windows 头文件中排除极少使用的内容

// 图片控件
//#define _AFXDLL
//#include <afxwin.h>

// Windows 头文件
#include <windows.h>
// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// 
//#include <exception>
//#include <thread>
// 打开对话框选择单个文件
#include <commdlg.h>
// 打开文件夹 、、 2021-12-26
#include <ShlObj.h>

// 播放音乐
#include <mmsystem.h>
#pragma comment(lib, "WINMM.LIB")
// 图片动画类
#pragma comment(lib,"Msimg32.lib")//添加使用TransparentBlt函数所需的库文件

//自定义类
#include "ImgButton.h"
#include "Flash.h"

// tooltips
//#include <windowsx.h>
#include <CommCtrl.h>
#pragma comment(lib, "comctl32.lib")

//#include <iostream>
//#include <string>
//using namespace std;
