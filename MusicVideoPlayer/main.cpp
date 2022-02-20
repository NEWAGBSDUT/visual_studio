#include "framework.h"
#include "main.h"

#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
BOOL                OpenSingleFile(HWND hWnd);// 选择并获取单个文件路径
VOID                ResourceDestory(HWND hWnd);
BOOL                ResourceInit(HINSTANCE hInstance, HWND hWnd);
ATOM                VideoAreaRegisterClass(HINSTANCE hInstance);
LRESULT CALLBACK    VideoWndProc(HWND, UINT, WPARAM, LPARAM);
void                VideoDisplay();
LRESULT CALLBACK    BTN_NEXT_WNDPROC(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    BTN_PREVIOUS_WNDPROC(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    BTN_PLAY_WNDPROC(HWND, UINT, WPARAM, LPARAM);

VOID                FlashThread(HWND hWnd);

void                pauseMusic(HWND hWnd);
void                playMusic(HWND hWnd);
void                playVideo(HWND hWnd);
void                AutoPlayThread(HWND hWnd);// 自动播放下一首线程
void                getPlayStatus(HWND hWnd);
void                start_auto_play_thread();
//2021-12-26 start
void                ChoicePlayPath(HWND hWnd);
//2021-12-26 end
void        updateMusicBuff(TCHAR* p); // 更新当前播放歌曲


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此处放置代码。
    // 2020-8-29 检查程序是否已有运行实例
    HANDLE m_hMutex = CreateMutex(NULL, TRUE, L"music_video_player_001");
    DWORD dwRet = GetLastError();
    if (m_hMutex) {
        if (ERROR_ALREADY_EXISTS == dwRet) {
            //printf("程序已经运行，退出！\n");
            CloseHandle(m_hMutex);
            m_hMutex = NULL;
            return 0;
        }
    }
    else {
        //printf("创建互斥量错误，程序退出！\n");
        if (NULL != m_hMutex) {
            CloseHandle(m_hMutex);
        }

        m_hMutex = NULL;
        return 0;
    }// 2020-8-29 end
    // 
    // 窗口居中显示
    WIN_MARGIN_WIDTH  = int((GetSystemMetrics(SM_CXSCREEN) - WIN_WIDTH) / 2);
    WIN_MARGIN_HEIGHT = int((GetSystemMetrics(SM_CYSCREEN) - WIN_HEIGHT) / 2 - 80);

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);
    //注册视频窗口类
    //VideoAreaRegisterClass(hInstance);

    // 执行应用程序初始化:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY));

    MSG msg;

    // 主消息循环:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    CloseHandle(m_hMutex);
    m_hMutex = NULL;
    return (int)msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY));
    wcex.hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR_HT)); //LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = CreateSolidBrush(RGB(223, 235, 248));
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_MY);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}
//
//  函数: VideoAreaRegisterClass()
//
//  目标: 注册视频窗口区域类。
//
ATOM VideoAreaRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW video_wcex;

    video_wcex.cbSize = sizeof(WNDCLASSEX);

    video_wcex.style         = CS_HREDRAW | CS_VREDRAW;
    video_wcex.lpfnWndProc   = VideoWndProc;
    video_wcex.cbClsExtra    = 0;
    video_wcex.cbWndExtra    = 0;
    video_wcex.hInstance     = hInstance;
    video_wcex.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY));
    video_wcex.hCursor       = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR_HT));
    video_wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    video_wcex.lpszMenuName  = NULL;
    video_wcex.lpszClassName = TEXT(VIDEO_CLASS_NAME);
    video_wcex.hIconSm       = LoadIcon(video_wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&video_wcex);
}
void VideoDisplay()
{
    SelectObject(VIDEO_BUFDC, VIDEO_BACKGROUND);
    BitBlt(VIDEO_MDC, 0, 0, VIDEO_WIDTH, VIDEO_HEIGHT, VIDEO_BUFDC, 0, 0, SRCCOPY);

    wchar_t* str = str2wchar("默认背景图片");
    SetTextColor(VIDEO_MDC, RGB(255, 255, 128));
    TextOut(VIDEO_MDC, 18, 2, str, (int)wcslen(str));

    BitBlt(VIDEO_HDC, 0, 0, VIDEO_WIDTH, VIDEO_HEIGHT, VIDEO_MDC, 0, 0, SRCCOPY);
}
//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // 将实例句柄存储在全局变量中

    HWND hWnd = CreateWindowW(szWindowClass, szTitle,
        WS_MINIMIZEBOX^WS_CLIPCHILDREN^ WS_CAPTION^ WS_SYSMENU,//WS_OVERLAPPEDWINDOW^ WS_THICKFRAME^ WS_MAXIMIZEBOX,//
        WIN_MARGIN_WIDTH, WIN_MARGIN_HEIGHT, WIN_WIDTH, WIN_HEIGHT, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }
    WIN_HWND = hWnd;
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
	SetWindowDisplayAffinity(hWnd, WDA_EXCLUDEFROMCAPTURE);// 防止截屏
    //三缓冲体系的创建
    srand((unsigned)time(NULL));            //用系统时间初始化随机种子
    HBITMAP bmp;
    WIN_HDC = GetDC(hWnd);
    WIN_MDC = CreateCompatibleDC(WIN_HDC);      //创建一个和hdc兼容的内存DC
    WIN_BUFDC = CreateCompatibleDC(WIN_HDC);
    bmp = CreateCompatibleBitmap(WIN_HDC, WIN_WIDTH, WIN_HEIGHT); //建一个和窗口兼容的空的位图对象

    SelectObject(WIN_MDC, bmp);               //将空位图对象放到mdc中
    GetClientRect(hWnd, &WIN_RECT);

    //设置字体
    HFONT hFont;
    hFont = CreateFont(20,             // 字号
        0, 0, 0,
        700,            // 在0到1000之间指定字体的权值，如400表示标准体，700表示黑（粗）体，如果此值为0，则使用缺省的权值。
        0,              // 如果设置为TRUE，则字体设置为斜体
        0,              // 如果设置为TRUE，则字体增加下划线。
        0,              // 如果设置为TRUE，则字体增加删除线。
        GB2312_CHARSET, // 指定字符集
        0, 0, 0,
        FF_SCRIPT,
        TEXT("楷体"));
    SelectObject(WIN_MDC, hFont);
    SetBkMode(WIN_MDC, TRANSPARENT);    //设置文字显示背景透明

    // 创建视频区域窗口并显示
    //VIDEO_HWND = CreateWindowW(
    //    TEXT(VIDEO_CLASS_NAME),
    //    TEXT(VIDEO_CLASS_NAME),
    //    WS_BORDER | WS_CHILD | WS_VISIBLE,
    //    VIDEO_MARGIN_WIDTH, VIDEO_MARGIN_HEIGHT, VIDEO_WIDTH, VIDEO_HEIGHT,
    //    hWnd, nullptr, hInstance, nullptr);
    //ShowWindow(VIDEO_HWND, nCmdShow);
    //UpdateWindow(VIDEO_HWND);
    //// 初始化资源
    //HBITMAP bmp1;
    ////三缓冲体系的创建
    //VIDEO_HDC   = GetDC(VIDEO_HWND);
    //VIDEO_MDC   = CreateCompatibleDC(VIDEO_HDC);      //创建一个和hdc兼容的内存DC
    //VIDEO_BUFDC = CreateCompatibleDC(VIDEO_HDC);    //再创建一个和hdc兼容的缓冲DC
    //bmp1 = CreateCompatibleBitmap(VIDEO_HDC, VIDEO_WIDTH, VIDEO_HEIGHT); //建一个和窗口兼容的空的位图对象

    //SelectObject(VIDEO_MDC, bmp1);               //将空位图对象放到mdc中
    //GetClientRect(VIDEO_HWND, &VIDEO_RECT);
    ////载入资源图到位图句柄中
    //VIDEO_BACKGROUND = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_VIDEO_AREA));
    ////设置字体
    //SelectObject(VIDEO_MDC, hFont);
    //SetBkMode(VIDEO_MDC, TRANSPARENT);    //设置文字显示背景透明
    //VideoDisplay();
    // 加载播放按钮
    btn_next->set_draw_btn_name(false);
    btn_next->draw_button(hWnd,
        BTN_NEXT_X, BTN_NEXT_Y,
        nCmdShow,
        hInstance,
        BTN_NEXT_WNDPROC);
    btn_previous->set_draw_btn_name(false);
    btn_previous->draw_button(hWnd,
        BTN_PREVIOUS_X, BTN_PREVIOUS_Y,
        nCmdShow,
        hInstance,
        BTN_PREVIOUS_WNDPROC);
    btn_play->set_draw_btn_name(false);
    btn_play->draw_button(hWnd,
        BTN_PLAY_X, BTN_PLAY_Y,
        nCmdShow,
        hInstance,
        BTN_PLAY_WNDPROC);
    btn_pause->set_draw_btn_name(false);
    btn_pause->draw_button(hWnd,
        BTN_PLAY_X, BTN_PLAY_Y,
        nCmdShow,
        hInstance,
        BTN_PLAY_WNDPROC);


    // 加载图片传送门
    std::thread ImagePortal_thread(FlashThread, hWnd);
    ImagePortal_thread.detach();

    // 主菜单资源初始化
    if (!ResourceInit(hInstance, hWnd))
    {
        return FALSE;
    }

    //2021-12-23 start
    //PlaySound(L"D:\\PROJECT\\vs2019\\MusicVideoPlayer\\MusicVideoPlayer\\source\\wonderfulLove.wav", hInstance, SND_FILENAME | SND_ASYNC | SND_LOOP);
    //2021-12-23 end

    return TRUE;
}

void getPlayStatus(HWND hWnd) {
    if (current_choice_song)
    {
        mciSendString(SONG_MODE, music_play_status, 255, 0);//USES_CONVERSION;  A2W()
        //TCHAR B[10] = { 0 };
        //wsprintf(B, L"%d", a);
        //MessageBox(hWnd, music_play_status, L"mode", MB_OK);
    }
}
void start_auto_play_thread()
{
    if (!exists_auto_play_thread) {
        std::thread auto_play_thread1(AutoPlayThread, WIN_HWND);
        auto_play_thread1.detach();        
        exists_auto_play_thread = true;
    }
}
void AutoPlayThread(HWND hWnd) {
    while (AUTOPLAY) {
        if(current_choice_song){
            if (music_play_flag) {
                playMusic(hWnd);
            }
            else if (!music_play_done && !music_play_flag) {
                pauseMusic(hWnd);
            }
        }
        else {
            playVideo(hWnd);
        }
        //getPlayStatus(hWnd);
        Sleep(AUTO_PLAY_SLEEP);
    }
}

VOID FlashThread(HWND hWnd)
{
    while (FLASHFLAG) {
        ImagePortal(hInst, WIN_RECT, WIN_MDC, WIN_BUFDC);
        StarFlash(hInst, WIN_RECT, WIN_MDC, WIN_BUFDC);
		// 写入文字
		swprintf_s(str_101, L"当前播放：");
		SetTextColor(WIN_MDC, RGB(255, 255, 0));
		TextOut(WIN_MDC, 10, 6, str_101, (int)wcslen(str_101));
		// 写入播放模式 start
		swprintf_s(str_101, L"播放模式");
		SetTextColor(WIN_MDC, RGB(255, 255, 0));
		TextOut(WIN_MDC, 400, 290, str_101, (int)wcslen(str_101));

		if (0 == play_cycle) {
			swprintf_s(str_101, L"顺序播放");
		}else if (1 == play_cycle) {
			swprintf_s(str_101, L"循环播放");
		}else if(2 == play_cycle) {
			swprintf_s(str_101, L"单曲循环");
		}else if(3 == play_cycle) {
			swprintf_s(str_101, L"随机播放");
		}else if(4 == play_cycle) {
			swprintf_s(str_101, L"单曲播放");
		}		
		SetTextColor(WIN_MDC, RGB(3, 41, 250));
		TextOut(WIN_MDC, 410, 310, str_101, (int)wcslen(str_101));
		// end

		SetTextColor(WIN_MDC, RGB(222, 3, 250));
		TextOut(WIN_MDC, 30, 26, current_play_music_name, (int)wcslen(current_play_music_name));
        //将mdc中的全部内容贴到hdc中
        BitBlt(WIN_HDC, 0, 0, WIN_WIDTH, WIN_HEIGHT, WIN_MDC, 0, 0, SRCCOPY);
        Sleep(FLASH_SLEEP);

    }    
}

LRESULT CALLBACK BTN_NEXT_WNDPROC(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: 在此处添加使用 hdc 的任何绘图代码...
        btn_next->display();
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_MOUSEHOVER:
    {
        btn_next->display_hover();
    }
    break;
    case WM_MOUSELEAVE:
    {
        btn_next->display();
    }
    break;
    case WM_MOUSEMOVE: // 鼠标移动消息
    {
        TRACKMOUSEEVENT tme;
        tme.cbSize = sizeof(TRACKMOUSEEVENT);
        tme.dwFlags = TME_LEAVE | TME_HOVER;
        tme.hwndTrack = hWnd;
        tme.dwHoverTime = 10;
        ::TrackMouseEvent(&tme);
    }
    break;
    case WM_LBUTTONDOWN: // 鼠标左键按下
    {
		if (play_cycle == 3)//随机播放
		{
			int ps = 0;
			ps = (rand() % (current_play_list_length - 1)) + 1;
			current_point = temp_point;
			current_play_list_index = 0;
			while (ps-- && *current_point) {
				current_point += lstrlen(current_point) + 1;
				current_play_list_index += 1;
			}
			if (*current_point)
			{
				*current_play = { 0 };
				*current_play_music_name = { 0 };
				lstrcat(current_play, current_play_path);  //给文件名加上路径  
				lstrcat(current_play, current_point);    //加上文件名  
				lstrcat(current_play_music_name, current_point);
				updateMusicBuff(current_play); 
				current_point += lstrlen(current_point) + 1;     //移至下一个文件
				current_play_list_index += 1;
			}
		}
		else {
			if (*current_point)
			{
				*current_play = { 0 };
				*current_play_music_name = { 0 };
				lstrcat(current_play, current_play_path);  //给文件名加上路径  
				lstrcat(current_play, current_point);    //加上文件名  
				lstrcat(current_play_music_name, current_point);
				updateMusicBuff(current_play);
				current_point += lstrlen(current_point) + 1;     //移至下一个文件
				current_play_list_index += 1;
			}
			else {
				current_point = temp_point;
				*current_play = { 0 };
				*current_play_music_name = { 0 };
				lstrcat(current_play, current_play_path);  //给文件名加上路径  
				lstrcat(current_play, current_point);    //加上文件名  
				lstrcat(current_play_music_name, current_point);
				updateMusicBuff(current_play);
				current_point += lstrlen(current_point) + 1;     //移至下一个文件
				current_play_list_index = 0;
			}
			
		}

    }
    break;
    case WM_LBUTTONUP: // 鼠标左键弹起
    {
        //menu_LMBDown = false;
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

LRESULT CALLBACK BTN_PREVIOUS_WNDPROC(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: 在此处添加使用 hdc 的任何绘图代码...
        btn_previous->display();
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_MOUSEHOVER:
    {
        btn_previous->display_hover();
    }
    break;
    case WM_MOUSELEAVE:
    {
        btn_previous->display();
    }
    break;
    case WM_MOUSEMOVE: // 鼠标移动消息
    {
        TRACKMOUSEEVENT tme;
        tme.cbSize = sizeof(TRACKMOUSEEVENT);
        tme.dwFlags = TME_LEAVE | TME_HOVER;
        tme.hwndTrack = hWnd;
        tme.dwHoverTime = 10;
        ::TrackMouseEvent(&tme);
    }
    break;
    case WM_LBUTTONDOWN: // 鼠标左键按下
    {
		if (play_cycle == 3)//随机播放
		{
			int ps = 0;
			ps = (rand() % (current_play_list_length - 1)) + 1;
			current_point = temp_point;
			current_play_list_index = 0;

			while (ps-- && *current_point) {
				current_point += lstrlen(current_point) + 1;
				current_play_list_index += 1;
			}
			if (*current_point)
			{
				*current_play = { 0 };
				*current_play_music_name = { 0 };
				lstrcat(current_play, current_play_path);  //给文件名加上路径  
				lstrcat(current_play, current_point);    //加上文件名  
				lstrcat(current_play_music_name, current_point);
				updateMusicBuff(current_play);
				current_point += lstrlen(current_point) + 1;     //移至下一个文件
				current_play_list_index += 1;
			}
		}
		else {
			current_point = temp_point;
			int ps = current_play_list_index;
			current_play_list_index = 0;
			if (ps == 0) {
				ps = (current_play_list_length);
			}
			ps -= 1;
			while (ps-- && *current_point) {
				current_point += lstrlen(current_point) + 1;
				current_play_list_index += 1;
			}

			if (*current_point)
			{
				*current_play = { 0 };
				*current_play_music_name = { 0 };
				lstrcat(current_play, current_play_path);  //给文件名加上路径  
				lstrcat(current_play, current_point);    //加上文件名  
				lstrcat(current_play_music_name, current_point);
				updateMusicBuff(current_play);
				current_point += lstrlen(current_point) + 1;     //移至下一个文件
				if (*current_point) {
				}
				else {
					current_point = temp_point;
				}
			}
		}


    }
    break;
    case WM_LBUTTONUP: // 鼠标左键弹起
    {
        //menu_LMBDown = false;
    }
    break;
    case WM_DESTROY:

        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
LRESULT CALLBACK BTN_PLAY_WNDPROC(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: 在此处添加使用 hdc 的任何绘图代码...
        if (music_play_flag || video_play_flag) {
            btn_pause->display();
        }else{
            btn_play->display();
        }
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_MOUSEHOVER:
    {
        if (music_play_flag || video_play_flag) {
            btn_pause->display_hover();
        }
        else {
            btn_play->display_hover();
        }
    }
    break;
    case WM_MOUSELEAVE:
    {
        if (music_play_flag || video_play_flag) {
            btn_pause->display();
        }
        else {
            btn_play->display();
        }
    }
    break;
    case WM_MOUSEMOVE: // 鼠标移动消息
    {
        TRACKMOUSEEVENT tme;
        tme.cbSize = sizeof(TRACKMOUSEEVENT);
        tme.dwFlags = TME_LEAVE | TME_HOVER;
        tme.hwndTrack = hWnd;
        tme.dwHoverTime = 10;
        ::TrackMouseEvent(&tme);
    }
    break;
    case WM_LBUTTONDOWN: // 鼠标左键按下
    {
        if (current_choice_song) {
            if (music_play_flag) {
                music_play_flag = false;
                music_play_new = false;
            }
            else if (!music_play_flag) {
                music_play_flag = true;
            }
            start_auto_play_thread();
        }
        
    }
    break;
    case WM_LBUTTONUP: // 鼠标左键弹起
    {
        //menu_LMBDown = false;
    }
    break;
    case WM_DESTROY:

        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

void setStop() {
    btn_play->display();
    GetCursorPos(&screen_point);
    SetCursorPos(screen_point.x + 1, screen_point.y + 1);//更改指针坐标
    SetCursorPos(screen_point.x, screen_point.y);//更改指针坐标
}
void pauseMusic(HWND hWnd) {    
    getPlayStatus(hWnd);
    if (!local_str_cm(STATUS_PAUSED, music_play_status)) {
        mciSendString(PAUSE_SONG, NULL, 0, NULL);
        btn_play->display();
        GetCursorPos(&screen_point);
        SetCursorPos(screen_point.x + 1, screen_point.y + 1);//更改指针坐标
        SetCursorPos(screen_point.x, screen_point.y);//更改指针坐标
    }
}
void playMusic(HWND hWnd)
{
    if (video_play_flag) { // 停止视频播放

    }
    if (music_play_new) {
        if(!local_str_cm(L"", music_buff)){
            mciSendString(STOP_SONG, NULL, 0, NULL);
            wchar_t buff[256] = { 0 };
            mciSendString(music_buff, buff, 256, NULL);
            //MessageBox(hWnd, buff, L"加载", MB_OK); // 加载多媒体的结果 1，normal
            mciSendString(PLAY_SONG, buff, 256, WIN_HWND);
            getPlayStatus(hWnd);
            // 获取歌曲长度
            wchar_t sPosition[256] = { 0 };
            mciSendString(SONG_LENGTH, sPosition, 255, 0);//USES_CONVERSION;  A2W()
            song_length = strtol(tchar2cchat(sPosition), NULL, 10);
            // 获取音量
            wchar_t chVolume[256];
            mciSendString(SONG_VOLUME, chVolume, 255, 0);
            song_volume = strtol(tchar2cchat(chVolume), NULL, 10);
            music_play_new = false;
            music_play_done = false;
            btn_pause->display();
            GetCursorPos(&screen_point);
            SetCursorPos(screen_point.x + 1, screen_point.y + 1);//更改指针坐标
            SetCursorPos(screen_point.x, screen_point.y);//更改指针坐标
        }
        else {
            music_play_flag = false;
            MessageBox(hWnd, L"当前没有打开任何文件或播放列表!", L"播放失败", MB_OK);
        }
    }else
    // 检查状态
    {
        getPlayStatus(hWnd);
        if (local_str_cm(STATUS_PAUSED, music_play_status)) {
            mciSendString(RESUME_SONG, NULL, 0, WIN_HWND);
            btn_pause->display();
            GetCursorPos(&screen_point);
            SetCursorPos(screen_point.x + 1, screen_point.y + 1);//更改指针坐标
            SetCursorPos(screen_point.x, screen_point.y);//更改指针坐标
        }
        else if (local_str_cm(STATUS_STOPPED, music_play_status)) {            
            music_play_done = true;
            music_play_new = true;
            
			if (0 == play_cycle) { //// 0、顺序播放
				if (*current_point)
				{
					*current_play = { 0 };
					*current_play_music_name = { 0 };
					lstrcat(current_play, current_play_path);  //给文件名加上路径  
					lstrcat(current_play, current_point);    //加上文件名  
					lstrcat(current_play_music_name, current_point);
					updateMusicBuff(current_play);

					current_point += lstrlen(current_point) + 1;     //移至下一个文件
					current_play_list_index += 1;
				}
				else {
					current_point = temp_point;

					*current_play = { 0 };
					*current_play_music_name = { 0 };
					lstrcat(current_play, current_play_path);  //给文件名加上路径  
					lstrcat(current_play, current_point);    //加上文件名  
					lstrcat(current_play_music_name, current_point);
					updateMusicBuff(current_play);

					current_point += lstrlen(current_point) + 1;     //移至下一个文件
					current_play_list_index = 0;

					music_play_flag = false;
					setStop();
				}
			}
			else if (1 == play_cycle) {//1、循环播放
				if (*current_point && current_play_list_length > 1)// && *next_point)
				{
					*current_play = { 0 };
					*current_play_music_name = { 0 };
					lstrcat(current_play, current_play_path);  //给文件名加上路径  
					lstrcat(current_play, current_point);    //加上文件名  
					lstrcat(current_play_music_name, current_point);
					updateMusicBuff(current_play);

					current_point += lstrlen(current_point) + 1;     //移至下一个文件
					current_play_list_index += 1;
				}
				else {
					current_point = temp_point;

					*current_play = { 0 };
					*current_play_music_name = { 0 };
					lstrcat(current_play, current_play_path);  //给文件名加上路径  
					lstrcat(current_play, current_point);    //加上文件名  
					lstrcat(current_play_music_name, current_point);
					updateMusicBuff(current_play);

					current_point += lstrlen(current_point) + 1;     //移至下一个文件
					current_play_list_index = 0;
				}
			}
			else if (2 == play_cycle) {//2、单曲循环
			}
			else if (3 == play_cycle) {//3、随机播放
				int ps = 0;
				ps = (rand() % (current_play_list_length - 1)) + 1;					
				current_point = temp_point;
				current_play_list_index = 0;
				while (ps-- && *current_point) {						
					current_point += lstrlen(current_point) + 1;
					current_play_list_index += 1;
				}
				if (*current_point)
				{
					*current_play = { 0 };
					*current_play_music_name = { 0 };
					lstrcat(current_play, current_play_path);  //给文件名加上路径  
					lstrcat(current_play, current_point);    //加上文件名  
					lstrcat(current_play_music_name, current_point);
					updateMusicBuff(current_play);
					current_point += lstrlen(current_point) + 1;     //移至下一个文件
					current_play_list_index += 1;
				}
			}
			else {
				music_play_flag = false;
				setStop();
			}
        }   
    }

    
    //MessageBox(hWnd, buff, L"加载", MB_OK);

    //PlaySound(TEXT("D:\example.wma"), hInstance, SND_FILENAME | SND_ASYNC | SND_LOOP); //循环播放背景音乐
    //PlaySound(MAKEINTRESOURCE(IDR_WAVE1), hInstance, SND_RESOURCE | SND_ASYNC | SND_LOOP); //循环播放背景音乐
}
void playVideo(HWND hWnd)
{

}

void updateMusicBuff(TCHAR* p)
{
	auto music_suffix = _tcsstr(p, _T("."));
	//if (p)
	//    _tprintf(_T("%.3s"), p);
	int music_suffix_len = wcslen(music_suffix);
	for (int i = 0; i < music_suffix_len; i++)
	{
		music_suffix[i] = ::towupper(music_suffix[i]);
	}
	//for (auto &str : music_suffix)
	//{
	//    str = ::towupper(str);
	//}
	// strcmp 请用wcscmp函数，你的字符串是宽字符串，要用宽字符串的函数来比较。
	if (local_str_cm(music_suffix, MP3) || local_str_cm(music_suffix, WMA)) {
		current_choice_song = true;
		music_play_new = true;
		
		wsprintf(music_buff, L"open \"%s\" alias %s parent %u Style %u notify", p, SONG_ALIAS, WIN_HWND, WS_CHILD);
	}
	else if (local_str_cm(music_suffix, M4A)) {
		current_choice_song = true;
		music_play_new = true;
	}
	else if (local_str_cm(music_suffix, FLAC)) {
		current_choice_song = true;
		music_play_new = true;
	}
	else if (local_str_cm(music_suffix, MP4)) {
		current_choice_song = false;

	}
	else if (local_str_cm(music_suffix, MKV)) {
		current_choice_song = false;
	}
	else if (local_str_cm(music_suffix, RMVB)) {
		current_choice_song = false;
	}
}

BOOL OpenSingleFile(HWND hWnd)
{
	*current_play = { 0 };
    //TCHAR szBuffer[MAX_PATH] = { 0 };
    OPENFILENAME ofn = { 0 };
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFilter = _T("所有支持音乐(*.mp3;*.wma;*.m4a;*.flac)\0*.mp3;*.wma;*.m4a;*.flac\0"
        "MP3(*.mp3)\0*.mp3\0"
        "WMA(*.wma)\0*.wma\0"
        "M4A(*.m4a)\0*.m4a\0"
        "FLAC(*.flac)\0*.flac\0"
        "所有支持视频(*.mp4;*.mkv;*.rmvb)\0*.mp4;*.mkv;*.rmvb\0"
        "MP4(*.mp4)\0*.mp4\0"
        "MKV(*.mkv)\0*.mkv\0"
        "RMVB(*.rmvb)\0*.rmvb\0"
        "所有文件(*.*)\0*.*\0");//要选择的文件后缀   
    ofn.lpstrInitialDir = _T("D:\\");//默认的文件路径
    ofn.lpstrFile = current_play;//存放文件的缓冲区   
    ofn.nMaxFile = sizeof(current_play) / sizeof(*current_play);
    ofn.nFilterIndex = 0;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;//标志如果是多选要加上OFN_ALLOWMULTISELECT  
    //BOOL bSel = GetOpenFileName(&ofn);
    if (!GetOpenFileName(&ofn)) { /*MessageBox(hWnd, L"文件打开失败！", L"警告", MB_OK);*/ return false; }
    else {
		*current_play_music_name = { 0 };
		lstrcpyn(current_play_path, current_play, ofn.nFileOffset);//lstrcpyn(szPath, szOpenFileNames, ofn.nFileOffset);

		current_play_path[ofn.nFileOffset] = '\0';
		int nLen = 0;
		nLen = lstrlen(current_play_path);

		if (current_play_path[nLen - 1] != '\\')   //如果选了多个文件,则必须加上'//'
		{
			lstrcat(current_play_path, TEXT("\\"));
		}
		current_point = current_play + ofn.nFileOffset;
		temp_point = current_point;
		lstrcat(current_play_music_name, current_point);

		current_play_list_length = 1;
		updateMusicBuff(current_play);
		play_cycle = 4;
        return true;
    }
}
//// 2021-12-26 start
void OpenMultiFile(HWND hWnd)
{    
	//*current_play_list = { 0 };
	
    //TCHAR szOpenFileNames[100 * MAX_PATH] = { 0 };
    //TCHAR szPath[MAX_PATH];
    
    //TCHAR *p;
    int nLen = 0;
    OPENFILENAME ofn;
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER | OFN_ALLOWMULTISELECT;
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hWnd;
	ofn.lpstrFile = current_play_list;// szOpenFileNames;
    ofn.nMaxFile = sizeof(current_play_list) / sizeof(*current_play_list);//sizeof(szOpenFileNames) / sizeof(*szOpenFileNames);
    //ofn.lpstrFile[0] = '/0';
    ofn.lpstrFilter = _T("所有支持音乐视频(*.mp3;*.wma;*.m4a;*.flac;*.mp4;*.mkv;*.rmvb)\0*.mp3;*.wma;*.m4a;*.flac;*.mp4;*.mkv;*.rmvb\0"
        "MP3(*.mp3)\0*.mp3\0"
        "WMA(*.wma)\0*.wma\0"
        "M4A(*.m4a)\0*.m4a\0"
        "FLAC(*.flac)\0*.flac\0"
        "MP4(*.mp4)\0*.mp4\0"
        "MKV(*.mkv)\0*.mkv\0"
        "RMVB(*.rmvb)\0*.rmvb\0");
    ofn.lpstrInitialDir = _T("D:\\");//默认的文件路径
    ofn.nFilterIndex = 0;
    if (GetOpenFileName(&ofn))
    {
		current_play_list_length = 0;
        //把第一个文件名前的复制到szPath,即:
        //如果只选了一个文件,就复制到最后一个'/'
        //如果选了多个文件,就复制到第一个NULL字符
        lstrcpyn(current_play_path, current_play_list, ofn.nFileOffset);//lstrcpyn(szPath, szOpenFileNames, ofn.nFileOffset);
        //当只选了一个文件时,下面这个NULL字符是必需的.
        //这里不区别对待选了一个和多个文件的情况
        /*szPath[ofn.nFileOffset] = '\0';
        nLen = lstrlen(szPath);*/
		current_play_path[ofn.nFileOffset] = '\0';
        nLen = lstrlen(current_play_path);

        if (current_play_path[nLen - 1] != '\\')   //如果选了多个文件,则必须加上'//'
        {
            lstrcat(current_play_path, TEXT("\\"));
        }

        //p = szOpenFileNames + ofn.nFileOffset; //把指针移到第一个文件
		current_point = current_play_list + ofn.nFileOffset;
		temp_point = current_point;

		if(*current_point){
			*current_play = { 0 };
			*current_play_music_name = { 0 };
			lstrcat(current_play, current_play_path);  //给文件名加上路径  
			lstrcat(current_play, current_point);    //加上文件名 
			lstrcat(current_play_music_name,current_point);
			updateMusicBuff(current_play);
			music_play_flag = true;
			start_auto_play_thread();

			current_point += lstrlen(current_point) + 1;     //移至下一个文件
			current_play_list_index = 0;
		}
        //ZeroMemory(current_play_list, sizeof(current_play_list));
		TCHAR* p;
		p = temp_point;
        while (*p)
        {
			current_play_list_length += 1;
            p += lstrlen(p) + 1;     //移至下一个文件
        }
		if (4 == play_cycle) {
			play_cycle = 0;
		}
        //MessageBox(hWnd, szFileName, L"MultiSelect", MB_OK);
    }
    else {
        //MessageBox(hWnd, TEXT("打开选择框失败！"), TEXT("MultiSelect"), MB_OK);
    }
}
void ChoicePlayPath(HWND hWnd)
{
    TCHAR path[MAX_PATH] = { 0 };
    BROWSEINFO bi;
    ZeroMemory(&bi, sizeof(BROWSEINFO));
    bi.hwndOwner = hWnd;
    bi.pszDisplayName = path;
    bi.lpszTitle = L"请选择播放路径";
    //bi.pidlRoot = _T("D:\\");
    bi.ulFlags = BIF_RETURNONLYFSDIRS;// BIF_BROWSEINCLUDEFILES
    LPITEMIDLIST idl = SHBrowseForFolder(&bi);
    if (NULL != idl) {
        SHGetPathFromIDList(idl, path);
        if (local_str_cm(path, L""))
        {
            MessageBox(hWnd, TEXT("打开选择框失败！"), TEXT("PathSelect"), MB_OK);
        }
        //MessageBox(hWnd, path, TEXT("已选择"), MB_OK);
    }
    else {
        //MessageBox(hWnd, TEXT("打开选择框失败！"), TEXT("PathSelect"), MB_OK);
    }
}
// 2021-12-26 end
//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // 分析菜单选择:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        case IDM_OPEN:
            if(OpenSingleFile(hWnd)){ 
                if(current_choice_song){
                    music_play_new = true;
                    music_play_flag = true;
                    start_auto_play_thread();
                }
            }
            
            //
            break;//2021-12-26 start
        case IDM_CREATE_PLAY_LIST:
        {
            OpenMultiFile(hWnd);
        }
        break;
        case IDM_CHOICE_PLAY_LIST:
        {
            ChoicePlayPath(hWnd);
            
            
        }break;//2021-12-26 end
		case IDM_CYCLE_CLOSED://0、顺序播放   
		{
			play_cycle = 0;
			cycle = false;
			random_play = false;
		}break;
		case IDM_CYCLE_LIST://1、循环播放
		{
			play_cycle = 1;
			cycle = true;
			random_play = false;
		}break;
		case IDM_CYCLE_SIGNAL://2、单曲循环
		{
			play_cycle = 2;
			cycle = false;
			random_play = false;
		}break;
		case IDM_CYCLE_RANDOM://3、随机播放
		{
			play_cycle = 3;
			cycle = true;
			random_play = true;
		}break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: 在此处添加使用 hdc 的任何绘图代码...
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_NCLBUTTONDOWN:
    {
        //MessageBox(hWnd, L"非客户区！", L"警告", MB_OK);
        DefWindowProc(hWnd, message, wParam, lParam);
    }
        break;
    case WM_MOUSEMOVE: // 鼠标移动消息
    {
        MOUSEX = LOWORD(lParam);
        MOUSEY = HIWORD(lParam);
        //GetWindowRect(hWnd, &WINDOW_RECT);
        //GetClientRect(hWnd, &WIN_RECT);
        //wsprintf(buf, L"WINDOW_RECT left %d right %d bottom %d top %d ", WINDOW_RECT.left, WINDOW_RECT.right, WINDOW_RECT.bottom, WINDOW_RECT.top);
        //MessageBox(hWnd, buf, L"整个窗口", MB_OK);
        //wsprintf(buf, L"WIN_RECT left %d right %d bottom %d top %d ", WIN_RECT.left, WIN_RECT.right, WIN_RECT.bottom, WIN_RECT.top);
        //MessageBox(hWnd, buf, L"客户区", MB_OK);
    }
    break;
    case WM_SETCURSOR:
    {
        GetCursorPos(&screen_point);
        //SetCursorPos(screen_point.x + 1, screen_point.y + 10);//更改指针坐标
        ScreenToClient(hWnd, &screen_point);
        //wsprintf(buf, L"WIN_RECT x %d y %d ", screen_point.x, screen_point.y);
        //MessageBox(hWnd, buf, L"客户区", MB_OK);
        if ((screen_point.y < -23) && (screen_point.x<362)&&(screen_point.x>28))
        {
            HCURSOR hcur = LoadCursor(NULL, IDC_SIZEALL);
            //HWND current_hwnd = GetWindow();//FindWindow(szWindowClass, szTitle);
            //HWND hwnd1 = GetDesktopWindow();// GetForegroundWindow();//GetActiveWindow(); //
            //HWND current_hwnd = GetParent(hwnd1);
            //while (1) {
            //    if (NULL == current_hwnd) {

            //        break;
            //    }
            //    else {
            //        hwnd1 = current_hwnd;
            //        current_hwnd = GetParent(current_hwnd);
            //    }
            //}
            SetCursor(hcur);
            //SetClassLong(NULL, GCL_HCURSOR, (long)hcur);//,(long)LoadCursorFromFile("mouse.cur"));

        }
        else {
            HCURSOR hcur = LoadCursor(hInst, MAKEINTRESOURCE(IDC_CURSOR_HT));
            SetCursor(hcur);
            //SetClassLong(hWnd, GCL_HCURSOR, (long)LoadCursor(hInst, MAKEINTRESOURCE(IDC_CURSOR_HT)));//,(long)LoadCursorFromFile("mouse.cur"));
        }
    }
        break;
    case WM_DESTROY:
        ResourceDestory(hWnd);
        PostQuitMessage(0);
        break;
    default:
    {
        
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    }
    return 0;
}

LRESULT CALLBACK VideoWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: 在此处添加使用 hdc 的任何绘图代码...
        VideoDisplay();
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:

        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

BOOL ResourceInit(HINSTANCE hInstance, HWND hWnd)
{
    MUSIC_PLAY_DIALOG = (HBITMAP)LoadImage(NULL, L"D:\\PROJECT\\vs2019\\HappyGame\\Source\\IMG\\2D\\paper2.bmp", IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
    return TRUE;
}

VOID ResourceDestory(HWND hWnd)
{
    FLASHFLAG = FALSE;
    AUTOPLAY  = FALSE;
    DeleteObject(MUSIC_PLAY_DIALOG);
    ImagePortalResourceDelete();
    StarFlash2DResourceDelete();
}