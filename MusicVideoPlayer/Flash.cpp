#include "Flash.h"

int                  slide_show_count = 0; // 已绘制图片数量
#define              SLIDE_SHOW_IMG_NUM       4    // img数量
HBITMAP		         SLIDE_SHOW_IMG[SLIDE_SHOW_IMG_NUM];
FLASH_OBJECT_STRUCT  FLASH_SHOW[SLIDE_SHOW_IMG_NUM];
int                  slide_show_resource_init = 0;

bool img_show = true; // 是否滑动图片
int lock_img = 0;
int lock_time = 0;
int lock_time_max = 60;
int cur_index = 0;
HBITMAP img_mask;

void ImagePortalResourceDelete()
{
	//--------------------------------
	for (int i = 0; i < SLIDE_SHOW_IMG_NUM; i++)
	{
		DeleteObject(SLIDE_SHOW_IMG[i]);
	}
	DeleteObject(img_mask);
	slide_show_resource_init = 0;
	//--------------------------------

}
void ImagePortalResourceInitial(HINSTANCE hInstance, RECT AppWindow)
{
	SLIDE_SHOW_IMG[0] = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP_IMAGEPORTAL2));
	SLIDE_SHOW_IMG[1] = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP_IMAGEPORTAL3));
	SLIDE_SHOW_IMG[2] = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP_IMAGEPORTAL4));
	SLIDE_SHOW_IMG[3] = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP_IMAGEPORTAL5));

	img_mask = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP_IMAGEPORTAL1));
	while (slide_show_count < SLIDE_SHOW_IMG_NUM)
	{
		FLASH_SHOW[slide_show_count].x = AppWindow.right - slide_show_count * 130;
		FLASH_SHOW[slide_show_count].y = AppWindow.bottom - 160 - slide_show_count * 30;
		FLASH_SHOW[slide_show_count].vx = 1;
		FLASH_SHOW[slide_show_count].vy = 30;
		FLASH_SHOW[slide_show_count].exist = true;
		slide_show_count++;
	}
	slide_show_resource_init = 1;
}
/*
* 图片传送动画
*/
void ImagePortal(HINSTANCE hInstance, RECT AppWindow, HDC MDC, HDC BUFDC)
{
	if (!slide_show_resource_init)
	{
		ImagePortalResourceInitial(hInstance, AppWindow);
	}
	SelectObject(BUFDC, img_mask);
	BitBlt(MDC, 0, 0, AppWindow.right, AppWindow.bottom, BUFDC, 0, 0, SRCCOPY);

	if (img_show)// 多张图片滑动
	{

		for (int i = 0; i < slide_show_count; i++)
		{
			if (FLASH_SHOW[i].exist)
			{
				SelectObject(BUFDC, SLIDE_SHOW_IMG[i]);
				TransparentBlt(MDC, FLASH_SHOW[i].x, FLASH_SHOW[i].y, 170, 123, BUFDC, 0, 0, 551, 400, RGB(0, 0, 0));
				FLASH_SHOW[i].x -= FLASH_SHOW[i].vx;

				if (FLASH_SHOW[i].x < -170)
				{
					FLASH_SHOW[i].x = AppWindow.right;
					FLASH_SHOW[i].y -= FLASH_SHOW[i].vy;
				}
				if (FLASH_SHOW[i].y < -45)
				{
					img_show = false;
				}
			}
		}
	}
	else {
		if (!lock_img)
		{
			cur_index = rand() % SLIDE_SHOW_IMG_NUM;
			lock_img = 1;
		}
		if (lock_time >= lock_time_max) {
			lock_img = 0;
			lock_time = 0;
		}
		else {
			lock_time++;
		}
		SelectObject(BUFDC, SLIDE_SHOW_IMG[cur_index]);
		TransparentBlt(MDC, 0, 0, AppWindow.right, AppWindow.bottom, BUFDC, 0, 0, 551, 400, RGB(0, 0, 0));
	}

	if (rand() % 10000 == 9999) {
		// 重新切入图片
		img_show = true;
	}

}
/*
* 星星闪烁
*/
#define             STAR_NUM 16 // 星星数量
int                 star_count = 0; // 已绘制星星数量
HBITMAP             STAR_IMG[STAR_NUM];
bool                star_img_resource_init = 0;  // 星星图片资源是否加载

FLASH_OBJECT_STRUCT STARFLASH[STAR_NUM];
int star_flash_count = 0; // 星星闪烁计数,一张图显示STARFLASH_flash_speed个循环
int star_flash_speed = 10; // 星星图片切换频率，越小表示闪烁越快 10

void StarFlash2DResourceDelete()
{
	//--------------------------------
	for (int i = 0; i < STAR_NUM; i++)
	{
		DeleteObject(STAR_IMG[i]);
	}
	star_img_resource_init = 0;
	//--------------------------------

}
// 二维星星闪烁动画资源加载
void StarFlash2DResourceInitial(HINSTANCE hInstance)
{
	STAR_IMG[0] = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP_STARFLASH8));
	STAR_IMG[1] = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP_STARFLASH9));
	STAR_IMG[2] = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP_STARFLASH6));
	STAR_IMG[3] = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP_STARFLASH7));

	star_img_resource_init = 1;

}

void StarFlash(HINSTANCE hInstance, RECT AppWindow, HDC MDC, HDC BUFDC)
{
	if (!star_img_resource_init) {
		StarFlash2DResourceInitial(hInstance);
	}

	if (star_count < STAR_NUM)               //当粒子数小于规定的粒子数时，便产生新的粒子，设定每个粒子的属性值
	{
		STARFLASH[star_count].x = rand() % AppWindow.right;  //将粒子的X坐标设为窗口中水平方向上的任意位置
		STARFLASH[star_count].y = rand() % AppWindow.bottom;  //将粒子的X坐标设为窗口中水平方向上的任意位置
		STARFLASH[star_count].exist = true;
		STARFLASH[star_count].vx = rand() % 4;
		STARFLASH[star_count].vy = 1;
		star_count++;                       //每产生一个粒子后进行累加计数
	}
	star_flash_count++;

	//首先判断粒子是否存在，若存在，进行透明贴图操作
	for (int i = 0; i < star_count; i++)
	{
		if (star_flash_count == star_flash_speed)
		{
			(STARFLASH[i].vx)++;
			if (STARFLASH[i].vx == 4)
				STARFLASH[i].vx = 0;
		}
		if (STARFLASH[i].exist)  //粒子还存在
		{
			//贴上粒子图
			SelectObject(BUFDC, STAR_IMG[STARFLASH[i].vx]);
			TransparentBlt(MDC, STARFLASH[i].x, STARFLASH[i].y, 64, 64, BUFDC, 0, 0, 64, 64, RGB(0, 0, 0));

			switch (rand() % 4)
			{
			case 0:
			{
				STARFLASH[i].x += 1;
				STARFLASH[i].y += 1;
			}
			break;
			case 1:
			{
				STARFLASH[i].x -= 1;
				STARFLASH[i].y -= 1;
			}
			break;
			case 2:
			{
				STARFLASH[i].x += 1;
				STARFLASH[i].y -= 1;
			}
			break;
			case 3:
			{
				STARFLASH[i].x -= 1;
				STARFLASH[i].y += 1;
			}
			break;
			default:
				break;
			}
			//越界检查
			if (STARFLASH[i].y > AppWindow.bottom)
			{
				STARFLASH[i].y = AppWindow.top;
			}
			if (STARFLASH[i].y < AppWindow.top)
			{
				STARFLASH[i].y = AppWindow.bottom;
			}
			if (STARFLASH[i].x > AppWindow.right)
			{
				STARFLASH[i].x = AppWindow.left;
			}
			if (STARFLASH[i].x < AppWindow.left)
			{
				STARFLASH[i].x = AppWindow.right;
			}
		}

	}
	if (star_flash_count == star_flash_speed)
		star_flash_count = 0;

}
/*
* 下雨动画
*/
void RainFlash(HWND hWnd)
{

}
/*
* 下雪动画
*/
void SnowFlash(HWND hWnd)
{

}