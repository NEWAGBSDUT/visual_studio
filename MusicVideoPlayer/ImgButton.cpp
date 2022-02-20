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
	wcex_btn.cbSize = sizeof(WNDCLASSEX); // �����ֽ�����С
	wcex_btn.style = CS_HREDRAW | CS_VREDRAW;
	wcex_btn.lpfnWndProc = click_function;// WndProc_Button; // ������Ϣ������
	wcex_btn.cbClsExtra = 0;
	wcex_btn.cbWndExtra = 0;
	wcex_btn.hInstance = hInstance; // ����suoshuʵ�����
	wcex_btn.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY)); // ��������ʾ�ĳ���ͼ��
	wcex_btn.hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR_HT)); // ���ָ��

	//wcex_btn.hCursor = LoadCursor(nullptr, IDC_ARROW); // ���ָ��
	wcex_btn.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
	wcex_btn.lpszMenuName = NULL; // MAKEINTRESOURCEW(IDC_HAPPYGAME); // ���ڵĲ˵�
	wcex_btn.lpszClassName = CString(class_name);
	wcex_btn.hIconSm = LoadIcon(wcex_btn.hInstance, MAKEINTRESOURCE(IDI_SMALL)); // �������Ͻǵ�ͼ��

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
	// ��ʾ�����´���
	ShowWindow(btn_hwnd, nCmdShow);
	UpdateWindow(btn_hwnd);

	if (!btn_hwnd)
	{
		return FALSE;
	}
	// ��ʼ����Դ
	HBITMAP bmp1;
	//��������ϵ�Ĵ���
	btn1_hdc = GetDC(btn_hwnd);
	btn1_mdc = CreateCompatibleDC(btn1_hdc);      //����һ����hdc���ݵ��ڴ�DC
	btn1_bufdc = CreateCompatibleDC(btn1_hdc);    //�ٴ���һ����hdc���ݵĻ���DC
	bmp1 = CreateCompatibleBitmap(btn1_hdc, w, h); //��һ���ʹ��ڼ��ݵĿյ�λͼ����

	SelectObject(btn1_mdc, bmp1);               //����λͼ����ŵ�mdc��
	GetClientRect(btn_hwnd, &btn1_rect);
	//����һϵ����Ϸ��Դͼ��λͼ�����
	btn_img = LoadBitmap(hInstance, MAKEINTRESOURCE(img_id));
	if (btn_hover) {
		btn_img_ = LoadBitmap(hInstance, MAKEINTRESOURCE(img_id_));
	}

	//��������
	HFONT hFont;
	hFont = CreateFont(18,             // �ֺ�
		0, 0, 0,
		700,            // ��0��1000֮��ָ�������Ȩֵ����400��ʾ��׼�壬700��ʾ�ڣ��֣��壬�����ֵΪ0����ʹ��ȱʡ��Ȩֵ��
		0,              // �������ΪTRUE������������Ϊб��
		0,              // �������ΪTRUE�������������»��ߡ�
		0,              // �������ΪTRUE������������ɾ���ߡ�
		GB2312_CHARSET, // ָ���ַ���
		0, 0, 0,
		FF_SCRIPT,
		TEXT("����"));
	SelectObject(btn1_mdc, hFont);
	SetBkMode(btn1_mdc, TRANSPARENT);    //����������ʾ����͸��
	
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
