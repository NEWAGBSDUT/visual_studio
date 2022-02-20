#include "ImgButton.h"

void ImageButton::set_hWnd(HWND parent_hwnd)
{
	hWnd = parent_hwnd;
}
void ImageButton::set_nCmdShow(int nCmdShow1)
{
	nCmdShow = nCmdShow1;
}
void ImageButton::set_hInstance(HINSTANCE hInstance1)
{
	hInstance = hInstance1;
}
void ImageButton::set_click_function(WNDPROC click_function1)
{
	click_function = click_function1;
}
void ImageButton::set_x(int x1)
{
	x = x1;
}
void ImageButton::set_y(int y1)
{
	y = y1;
}

void ImageButton::set_draw_btn_name(bool b1)
{
	draw_btn_name = b1;
}

ImageButton::ImageButton(void) {}

ImageButton::ImageButton(string class_name1, string btn_name1,	int w1, int h1, int img_id1)
{
	strcpy_s(class_name, (class_name1).c_str());
	strcpy_s(btn_name, (btn_name1).c_str());

	w = w1;
	h = h1;
	img_id = img_id1;
}

ImageButton::ImageButton(string class_name1, string btn_name1, int w1, int h1, int* img_ids)
{
	strcpy_s(class_name, (class_name1).c_str());
	strcpy_s(btn_name, (btn_name1).c_str());

	w = w1;
	h = h1;

	img_id = img_ids[0];
	img_id_ = img_ids[1];

	btn_hover = 1;
}


void ImageButton::draw_button(HWND parent_hwnd,	int x, int y,	int nCmdShow1,	HINSTANCE hInstance1, 	WNDPROC click_function1) 
{
	this->set_x(x);
	this->set_y(y);
	this->set_click_function(click_function1);
	this->set_hInstance(hInstance1);
	this->set_hWnd(parent_hwnd);
	this->set_nCmdShow(nCmdShow1);

	Register(hInstance, click_function);
	init(hInstance);
}

ImageButton::~ImageButton(void) {
	DeleteObject(btn_img);
	DeleteDC(btn1_bufdc);
	DeleteDC(btn1_mdc);
	ReleaseDC(btn_hwnd, btn1_hdc);
}

ATOM ImageButton::Register(HINSTANCE hInstance, WNDPROC click_function)
{
	
	WNDCLASSEXW wcex_btn;
	wcex_btn.cbSize = sizeof(WNDCLASSEX); // 窗口字节数大小
	wcex_btn.style = CS_HREDRAW | CS_VREDRAW;
	wcex_btn.lpfnWndProc = click_function;// WndProc_Button; // 窗口消息处理函数
	wcex_btn.cbClsExtra = 0;
	wcex_btn.cbWndExtra = 0;
	wcex_btn.hInstance = hInstance; // 窗口suoshu实例句柄
	wcex_btn.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY)); // 任务栏显示的程序图标
	wcex_btn.hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR_HT)); // 鼠标指针

	//wcex_btn.hCursor = LoadCursor(nullptr, IDC_ARROW); // 鼠标指针
	wcex_btn.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
	wcex_btn.lpszMenuName = NULL; // MAKEINTRESOURCEW(IDC_HAPPYGAME); // 窗口的菜单
	wcex_btn.lpszClassName = CString(class_name);
	wcex_btn.hIconSm = LoadIcon(wcex_btn.hInstance, MAKEINTRESOURCE(IDI_SMALL)); // 窗口左上角的图标

	return RegisterClassExW(&wcex_btn);
}


BOOL ImageButton::init(HINSTANCE hInstance)
{
	btn_hwnd = CreateWindowW(
		CString(class_name),   // predefined class  
		CString(btn_name),       // button text  
		WS_BORDER | WS_CHILD | WS_VISIBLE,// styles
		// Size and position values are given explicitly, because  
		// the CW_USEDEFAULT constant gives zero values for buttons.  
		x,         // starting x position
		y,         // starting y position  
		w,        // button width  
		h,        // button height  
		hWnd,       // parent window  
		nullptr,       // No menu GWL_HINSTANCE
		hInstance,//(HINSTANCE)GetWindowLong(hWnd, (-6)),
		nullptr);
	// 显示并更新窗口
	ShowWindow(btn_hwnd, nCmdShow);
	UpdateWindow(btn_hwnd);

	if (!btn_hwnd)
	{
		return FALSE;
	}
	// 初始化资源
	HBITMAP bmp1;
	//三缓冲体系的创建
	btn1_hdc = GetDC(btn_hwnd);
	btn1_mdc = CreateCompatibleDC(btn1_hdc);      //创建一个和hdc兼容的内存DC
	btn1_bufdc = CreateCompatibleDC(btn1_hdc);    //再创建一个和hdc兼容的缓冲DC
	bmp1 = CreateCompatibleBitmap(btn1_hdc, w, h); //建一个和窗口兼容的空的位图对象

	SelectObject(btn1_mdc, bmp1);               //将空位图对象放到mdc中
	GetClientRect(btn_hwnd, &btn1_rect);
	//载入一系列游戏资源图到位图句柄中
	btn_img = LoadBitmap(hInstance, MAKEINTRESOURCE(img_id));
	if (btn_hover) {
		btn_img_ = LoadBitmap(hInstance, MAKEINTRESOURCE(img_id_));
	}

	//设置字体
	HFONT hFont;
	hFont = CreateFont(18,             // 字号
		0, 0, 0,
		700,            // 在0到1000之间指定字体的权值，如400表示标准体，700表示黑（粗）体，如果此值为0，则使用缺省的权值。
		0,              // 如果设置为TRUE，则字体设置为斜体
		0,              // 如果设置为TRUE，则字体增加下划线。
		0,              // 如果设置为TRUE，则字体增加删除线。
		GB2312_CHARSET, // 指定字符集
		0, 0, 0,
		FF_SCRIPT,
		TEXT("楷体"));
	SelectObject(btn1_mdc, hFont);
	SetBkMode(btn1_mdc, TRANSPARENT);    //设置文字显示背景透明
	
	display();
	return TRUE;
}

void ImageButton::display() {
	SelectObject(btn1_bufdc, btn_img);
	BitBlt(btn1_mdc, 0, 0, w, h, btn1_bufdc, 0, 0, SRCCOPY);

	if (draw_btn_name)
	{
		wchar_t* str = str2wchar(btn_name);
		SetTextColor(btn1_mdc, RGB(255, 255, 128));
		TextOut(btn1_mdc, 18, 2, str, (int)wcslen(str));
	}

	BitBlt(btn1_hdc, 0, 0, w, h, btn1_mdc, 0, 0, SRCCOPY);
}

HWND ImageButton::get_hwnd() {
	return this->hWnd;
}

void ImageButton::display_hover() {
	SelectObject(btn1_bufdc, btn_img_);
	BitBlt(btn1_mdc, 0, 0, w, h, btn1_bufdc, 0, 0, SRCCOPY);

	if (draw_btn_name)
	{
		wchar_t* str = str2wchar(btn_name);
		SetTextColor(btn1_mdc, RGB(255, 255, 128));
		TextOut(btn1_mdc, 18, 2, str, (int)wcslen(str));
	}

	BitBlt(btn1_hdc, 0, 0, w, h, btn1_mdc, 0, 0, SRCCOPY);
}
