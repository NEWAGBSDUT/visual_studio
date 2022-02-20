#pragma once
#include "resource.h"

// 加载官方 Lib
//#pragma comment(lib,"Msimg32.lib") //添加使用TransparentBlt函数所需的库文件
#include <atlstr.h>

struct FLASH_OBJECT_STRUCT
{
	int x; // X坐标 4个字节对齐
	int y; // Y坐标 4个字节对齐
	int vx; // 水平方向速度 4个字节对齐
	int vy; // 垂直方向速度 4个字节对齐
	bool exist;  //是否存在 1+3个字节对齐
};


/*
#define IDB_BITMAP_IMAGEPORTAL1                     500
#define IDB_BITMAP_IMAGEPORTAL2                     501
#define IDB_BITMAP_IMAGEPORTAL3                     502
#define IDB_BITMAP_IMAGEPORTAL4                     503
#define IDB_BITMAP_IMAGEPORTAL5                     504
#define IDB_BITMAP_STARFLASH6                     505
#define IDB_BITMAP_STARFLASH7                     506
#define IDB_BITMAP_STARFLASH8                     507
#define IDB_BITMAP_STARFLASH9                     508

*
IDB_BITMAP_IMAGEPORTAL1             BITMAP                  "source\\IMG\\2D\\imageportal_bg.bmp"

IDB_BITMAP_IMAGEPORTAL2             BITMAP                  "source\\IMG\\2D\\imageportal_paper1.bmp"

IDB_BITMAP_IMAGEPORTAL3             BITMAP                  "source\\IMG\\2D\\imageportal_paper2.bmp"

IDB_BITMAP_IMAGEPORTAL4             BITMAP                  "source\\IMG\\2D\\imageportal_paper3.bmp"

IDB_BITMAP_IMAGEPORTAL5             BITMAP                  "source\\IMG\\2D\\imageportal_paper4.bmp"

IDB_BITMAP_STARFLASH6                     BITMAP       "source\\IMG\\2D\\star1.bmp"
IDB_BITMAP_STARFLASH7                     BITMAP       "source\\IMG\\2D\\star2.bmp"
IDB_BITMAP_STARFLASH8                     BITMAP       "source\\IMG\\2D\\star3.bmp"
IDB_BITMAP_STARFLASH9                     BITMAP       "source\\IMG\\2D\\star4.bmp"
*/


// 图片传送动画 图片资源回收--------------------------
void ImagePortalResourceDelete();
// 图片传送动画 图片资源初始化
void ImagePortalResourceInitial(HINSTANCE hInstance, RECT AppWindow);
/**随机图片切换 动画效果 图片传送动画
* @Param hInstance  主程序唯一句柄
* @Param AppWindow 一个RECT结构体，用于储存内部窗口区域的坐标,即程序窗口大小
* @Param HDC MDC; // 和hdc兼容的内存DC
* @Param HDC BUFDC; // 和hdc兼容的缓冲DC
*/
void ImagePortal(HINSTANCE hInstance, RECT AppWindow, HDC MDC, HDC BUFDC);



void StarFlash2DResourceDelete();
void StarFlash2DResourceInitial(HINSTANCE);
/**星星闪烁,二维动画效果
* @Param hInstance  主程序唯一句柄
* @Param AppWindow 一个RECT结构体，用于储存内部窗口区域的坐标,即程序窗口大小
* @Param HDC MDC; // 和hdc兼容的内存DC
* @Param HDC BUFDC; // 和hdc兼容的缓冲DC
*/
void StarFlash(HINSTANCE hInstance, RECT AppWindow, HDC MDC, HDC BUFDC);

/*
* 下雨动画
*/
void RainFlash(HWND hWnd);
/*
* 下雪动画
*/
void SnowFlash(HWND hWnd);