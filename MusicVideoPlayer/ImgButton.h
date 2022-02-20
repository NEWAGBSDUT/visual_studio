#pragma once
#include <windows.h>
#include "resource.h"
#include <thread>
#include <atlstr.h>
#include "StringUtil.h"
#include <string>
using namespace std;

class ImageButton
{

private:
	HINSTANCE hInstance;      // APPӦ�ó���Ψһʵ�����
	HWND      hWnd;           // ��ť���ڸ�����
	char      class_name[30] = { 0 }; // ��ť����
	char      btn_name[50] = { 0 };   // ��ť����
	HDC       btn1_hdc;  // ��ť---hdc
	HDC       btn1_mdc; // ��hdc���ݵ��ڴ�DC
	HDC       btn1_bufdc; // ��hdc���ݵĻ���DC
	RECT      btn1_rect; //����һ��RECT�ṹ�壬���ڴ����ڲ��������������,�����򴰿ڴ�С
	int x; // ��ť���Ͻ����� x
	int y; // ��ť���Ͻ����� y
	int w;
	int h;

	int nCmdShow;
	HBITMAP btn_img; // ��ťͼƬ
	HBITMAP btn_img_; // ��ťͼƬ�������ͣ��
	int btn_hover = 0;
	int img_id;      // ʹ���ߴ��ݵİ�ťͼƬ��ԴID
	int img_id_;      // ʹ���ߴ��ݵİ�ťͼƬ��ԴID
	HWND btn_hwnd; // ��ť���ھ��
	WNDPROC click_function;   // ��ť�¼�
	bool draw_btn_name = false;  // �Ƿ���ʾ��ť���ƣ�Ĭ�ϲ���ʾ��ֻʹ��ͼƬ

	ATOM Register(HINSTANCE, WNDPROC click_function); // ע�ᰴť����
	BOOL init(HINSTANCE);  // ��ʼ����ť������Դ

	void set_hWnd(HWND parent_hwnd);
	void set_nCmdShow(int nCmdShow1);
	void set_hInstance(HINSTANCE hInstance1);
	void set_click_function(WNDPROC click_function1);
	void set_x(int);
	void set_y(int);
	

public:

	void set_draw_btn_name(bool);

	// ��ʾ������ϵİ�ť
	void display();
	void display_hover();
	HWND get_hwnd();
	/*���ذ�ť������Դ �����ư�ť
	@param hwnd ��ť���ڸ�����
	@param ��ť���Ͻ����� x	 
	@param ��ť���Ͻ����� y
	@param
	@param ����Ψһ���
	@param ��ť��Ӧ�¼�
	*/
	void draw_button(HWND, int, int, int, HINSTANCE, WNDPROC);

	ImageButton(void);
	/* ����һ������ ͼƬ��ť��
	* @param class_name ��ť ����
	* @param btn_name ��ť ����
	* @param w ��
	* @param h ��
	* @param img_id ��ťͼƬ id
	*/
	ImageButton(string class_name, string btn_name, int w, int h, int img_id);
	/* ����һ������ ͼƬ��ť��
	* @param class_name ��ť ����
	* @param btn_name ��ť ����
	* @param w ��
	* @param h ��
	* @param img_ids ���Ű�ťͼƬ id
	*/
	ImageButton(string class_name, string btn_name, int w, int h, int* img_ids);

	/* ��������
	* �ͷ���Ч��ť��Դ
	*/
	~ImageButton(void);


};