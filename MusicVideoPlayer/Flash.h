#pragma once
#include "resource.h"

// ���عٷ� Lib
//#pragma comment(lib,"Msimg32.lib") //���ʹ��TransparentBlt��������Ŀ��ļ�
#include <atlstr.h>

struct FLASH_OBJECT_STRUCT
{
	int x; // X���� 4���ֽڶ���
	int y; // Y���� 4���ֽڶ���
	int vx; // ˮƽ�����ٶ� 4���ֽڶ���
	int vy; // ��ֱ�����ٶ� 4���ֽڶ���
	bool exist;  //�Ƿ���� 1+3���ֽڶ���
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


// ͼƬ���Ͷ��� ͼƬ��Դ����--------------------------
void ImagePortalResourceDelete();
// ͼƬ���Ͷ��� ͼƬ��Դ��ʼ��
void ImagePortalResourceInitial(HINSTANCE hInstance, RECT AppWindow);
/**���ͼƬ�л� ����Ч�� ͼƬ���Ͷ���
* @Param hInstance  ������Ψһ���
* @Param AppWindow һ��RECT�ṹ�壬���ڴ����ڲ��������������,�����򴰿ڴ�С
* @Param HDC MDC; // ��hdc���ݵ��ڴ�DC
* @Param HDC BUFDC; // ��hdc���ݵĻ���DC
*/
void ImagePortal(HINSTANCE hInstance, RECT AppWindow, HDC MDC, HDC BUFDC);



void StarFlash2DResourceDelete();
void StarFlash2DResourceInitial(HINSTANCE);
/**������˸,��ά����Ч��
* @Param hInstance  ������Ψһ���
* @Param AppWindow һ��RECT�ṹ�壬���ڴ����ڲ��������������,�����򴰿ڴ�С
* @Param HDC MDC; // ��hdc���ݵ��ڴ�DC
* @Param HDC BUFDC; // ��hdc���ݵĻ���DC
*/
void StarFlash(HINSTANCE hInstance, RECT AppWindow, HDC MDC, HDC BUFDC);

/*
* ���궯��
*/
void RainFlash(HWND hWnd);
/*
* ��ѩ����
*/
void SnowFlash(HWND hWnd);