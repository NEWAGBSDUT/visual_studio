#include "Flash.h"

int                  slide_show_count = 0; // �ѻ���ͼƬ����
#define              SLIDE_SHOW_IMG_NUM       4    // img����
HBITMAP		         SLIDE_SHOW_IMG[SLIDE_SHOW_IMG_NUM];
FLASH_OBJECT_STRUCT  FLASH_SHOW[SLIDE_SHOW_IMG_NUM];
int                  slide_show_resource_init = 0;

bool img_show = true; // �Ƿ񻬶�ͼƬ
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
* ͼƬ���Ͷ���
*/
void ImagePortal(HINSTANCE hInstance, RECT AppWindow, HDC MDC, HDC BUFDC)
{
	if (!slide_show_resource_init)
	{
		ImagePortalResourceInitial(hInstance, AppWindow);
	}
	SelectObject(BUFDC, img_mask);
	BitBlt(MDC, 0, 0, AppWindow.right, AppWindow.bottom, BUFDC, 0, 0, SRCCOPY);

	if (img_show)// ����ͼƬ����
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
		// ��������ͼƬ
		img_show = true;
	}

}
/*
* ������˸
*/
#define             STAR_NUM 16 // ��������
int                 star_count = 0; // �ѻ�����������
HBITMAP             STAR_IMG[STAR_NUM];
bool                star_img_resource_init = 0;  // ����ͼƬ��Դ�Ƿ����

FLASH_OBJECT_STRUCT STARFLASH[STAR_NUM];
int star_flash_count = 0; // ������˸����,һ��ͼ��ʾSTARFLASH_flash_speed��ѭ��
int star_flash_speed = 10; // ����ͼƬ�л�Ƶ�ʣ�ԽС��ʾ��˸Խ�� 10

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
// ��ά������˸������Դ����
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

	if (star_count < STAR_NUM)               //��������С�ڹ涨��������ʱ��������µ����ӣ��趨ÿ�����ӵ�����ֵ
	{
		STARFLASH[star_count].x = rand() % AppWindow.right;  //�����ӵ�X������Ϊ������ˮƽ�����ϵ�����λ��
		STARFLASH[star_count].y = rand() % AppWindow.bottom;  //�����ӵ�X������Ϊ������ˮƽ�����ϵ�����λ��
		STARFLASH[star_count].exist = true;
		STARFLASH[star_count].vx = rand() % 4;
		STARFLASH[star_count].vy = 1;
		star_count++;                       //ÿ����һ�����Ӻ�����ۼӼ���
	}
	star_flash_count++;

	//�����ж������Ƿ���ڣ������ڣ�����͸����ͼ����
	for (int i = 0; i < star_count; i++)
	{
		if (star_flash_count == star_flash_speed)
		{
			(STARFLASH[i].vx)++;
			if (STARFLASH[i].vx == 4)
				STARFLASH[i].vx = 0;
		}
		if (STARFLASH[i].exist)  //���ӻ�����
		{
			//��������ͼ
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
			//Խ����
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
* ���궯��
*/
void RainFlash(HWND hWnd)
{

}
/*
* ��ѩ����
*/
void SnowFlash(HWND hWnd)
{

}