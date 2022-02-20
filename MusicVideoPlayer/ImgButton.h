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
	HINSTANCE hInstance;      // APP应用程序唯一实例句柄
	HWND      hWnd;           // 按钮所在父窗口
	char      class_name[30] = { 0 }; // 按钮类名
	char      btn_name[50] = { 0 };   // 按钮名称
	HDC       btn1_hdc;  // 按钮---hdc
	HDC       btn1_mdc; // 和hdc兼容的内存DC
	HDC       btn1_bufdc; // 和hdc兼容的缓冲DC
	RECT      btn1_rect; //定义一个RECT结构体，用于储存内部窗口区域的坐标,即程序窗口大小
	int x; // 按钮左上角坐标 x
	int y; // 按钮左上角坐标 y
	int w;
	int h;

	int nCmdShow;
	HBITMAP btn_img; // 按钮图片
	HBITMAP btn_img_; // 按钮图片（鼠标悬停）
	int btn_hover = 0;
	int img_id;      // 使用者传递的按钮图片资源ID
	int img_id_;      // 使用者传递的按钮图片资源ID
	HWND btn_hwnd; // 按钮窗口句柄
	WNDPROC click_function;   // 按钮事件
	bool draw_btn_name = false;  // 是否显示按钮名称，默认不显示，只使用图片

	ATOM Register(HINSTANCE, WNDPROC click_function); // 注册按钮窗口
	BOOL init(HINSTANCE);  // 初始化按钮窗口资源

	void set_hWnd(HWND parent_hwnd);
	void set_nCmdShow(int nCmdShow1);
	void set_hInstance(HINSTANCE hInstance1);
	void set_click_function(WNDPROC click_function1);
	void set_x(int);
	void set_y(int);
	

public:

	void set_draw_btn_name(bool);

	// 显示绘制完毕的按钮
	void display();
	void display_hover();
	HWND get_hwnd();
	/*加载按钮所有资源 并绘制按钮
	@param hwnd 按钮所在父窗口
	@param 按钮左上角坐标 x	 
	@param 按钮左上角坐标 y
	@param
	@param 程序唯一句柄
	@param 按钮响应事件
	*/
	void draw_button(HWND, int, int, int, HINSTANCE, WNDPROC);

	ImageButton(void);
	/* 定义一个基本 图片按钮，
	* @param class_name 按钮 类名
	* @param btn_name 按钮 名称
	* @param w 宽
	* @param h 高
	* @param img_id 按钮图片 id
	*/
	ImageButton(string class_name, string btn_name, int w, int h, int img_id);
	/* 定义一个基本 图片按钮，
	* @param class_name 按钮 类名
	* @param btn_name 按钮 名称
	* @param w 宽
	* @param h 高
	* @param img_ids 两张按钮图片 id
	*/
	ImageButton(string class_name, string btn_name, int w, int h, int* img_ids);

	/* 析构方法
	* 释放无效按钮资源
	*/
	~ImageButton(void);


};