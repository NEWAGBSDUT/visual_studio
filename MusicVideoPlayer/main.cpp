#include "framework.h"
#include "main.h"

#define MAX_LOADSTRING 100

// ȫ�ֱ���:
HINSTANCE hInst;                                // ��ǰʵ��
WCHAR szTitle[MAX_LOADSTRING];                  // �������ı�
WCHAR szWindowClass[MAX_LOADSTRING];            // ����������

// �˴���ģ���а����ĺ�����ǰ������:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
BOOL                OpenSingleFile(HWND hWnd);// ѡ�񲢻�ȡ�����ļ�·��
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
void                AutoPlayThread(HWND hWnd);// �Զ�������һ���߳�
void                getPlayStatus(HWND hWnd);
void                start_auto_play_thread();
//2021-12-26 start
void                ChoicePlayPath(HWND hWnd);
//2021-12-26 end
void        updateMusicBuff(TCHAR* p); // ���µ�ǰ���Ÿ���


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: �ڴ˴����ô��롣
    // 2020-8-29 �������Ƿ���������ʵ��
    HANDLE m_hMutex = CreateMutex(NULL, TRUE, L"music_video_player_001");
    DWORD dwRet = GetLastError();
    if (m_hMutex) {
        if (ERROR_ALREADY_EXISTS == dwRet) {
            //printf("�����Ѿ����У��˳���\n");
            CloseHandle(m_hMutex);
            m_hMutex = NULL;
            return 0;
        }
    }
    else {
        //printf("�������������󣬳����˳���\n");
        if (NULL != m_hMutex) {
            CloseHandle(m_hMutex);
        }

        m_hMutex = NULL;
        return 0;
    }// 2020-8-29 end
    // 
    // ���ھ�����ʾ
    WIN_MARGIN_WIDTH  = int((GetSystemMetrics(SM_CXSCREEN) - WIN_WIDTH) / 2);
    WIN_MARGIN_HEIGHT = int((GetSystemMetrics(SM_CYSCREEN) - WIN_HEIGHT) / 2 - 80);

    // ��ʼ��ȫ���ַ���
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);
    //ע����Ƶ������
    //VideoAreaRegisterClass(hInstance);

    // ִ��Ӧ�ó����ʼ��:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY));

    MSG msg;

    // ����Ϣѭ��:
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
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
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
//  ����: VideoAreaRegisterClass()
//
//  Ŀ��: ע����Ƶ���������ࡣ
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

    wchar_t* str = str2wchar("Ĭ�ϱ���ͼƬ");
    SetTextColor(VIDEO_MDC, RGB(255, 255, 128));
    TextOut(VIDEO_MDC, 18, 2, str, (int)wcslen(str));

    BitBlt(VIDEO_HDC, 0, 0, VIDEO_WIDTH, VIDEO_HEIGHT, VIDEO_MDC, 0, 0, SRCCOPY);
}
//
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��:
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

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
	SetWindowDisplayAffinity(hWnd, WDA_EXCLUDEFROMCAPTURE);// ��ֹ����
    //��������ϵ�Ĵ���
    srand((unsigned)time(NULL));            //��ϵͳʱ���ʼ���������
    HBITMAP bmp;
    WIN_HDC = GetDC(hWnd);
    WIN_MDC = CreateCompatibleDC(WIN_HDC);      //����һ����hdc���ݵ��ڴ�DC
    WIN_BUFDC = CreateCompatibleDC(WIN_HDC);
    bmp = CreateCompatibleBitmap(WIN_HDC, WIN_WIDTH, WIN_HEIGHT); //��һ���ʹ��ڼ��ݵĿյ�λͼ����

    SelectObject(WIN_MDC, bmp);               //����λͼ����ŵ�mdc��
    GetClientRect(hWnd, &WIN_RECT);

    //��������
    HFONT hFont;
    hFont = CreateFont(20,             // �ֺ�
        0, 0, 0,
        700,            // ��0��1000֮��ָ�������Ȩֵ����400��ʾ��׼�壬700��ʾ�ڣ��֣��壬�����ֵΪ0����ʹ��ȱʡ��Ȩֵ��
        0,              // �������ΪTRUE������������Ϊб��
        0,              // �������ΪTRUE�������������»��ߡ�
        0,              // �������ΪTRUE������������ɾ���ߡ�
        GB2312_CHARSET, // ָ���ַ���
        0, 0, 0,
        FF_SCRIPT,
        TEXT("����"));
    SelectObject(WIN_MDC, hFont);
    SetBkMode(WIN_MDC, TRANSPARENT);    //����������ʾ����͸��

    // ������Ƶ���򴰿ڲ���ʾ
    //VIDEO_HWND = CreateWindowW(
    //    TEXT(VIDEO_CLASS_NAME),
    //    TEXT(VIDEO_CLASS_NAME),
    //    WS_BORDER | WS_CHILD | WS_VISIBLE,
    //    VIDEO_MARGIN_WIDTH, VIDEO_MARGIN_HEIGHT, VIDEO_WIDTH, VIDEO_HEIGHT,
    //    hWnd, nullptr, hInstance, nullptr);
    //ShowWindow(VIDEO_HWND, nCmdShow);
    //UpdateWindow(VIDEO_HWND);
    //// ��ʼ����Դ
    //HBITMAP bmp1;
    ////��������ϵ�Ĵ���
    //VIDEO_HDC   = GetDC(VIDEO_HWND);
    //VIDEO_MDC   = CreateCompatibleDC(VIDEO_HDC);      //����һ����hdc���ݵ��ڴ�DC
    //VIDEO_BUFDC = CreateCompatibleDC(VIDEO_HDC);    //�ٴ���һ����hdc���ݵĻ���DC
    //bmp1 = CreateCompatibleBitmap(VIDEO_HDC, VIDEO_WIDTH, VIDEO_HEIGHT); //��һ���ʹ��ڼ��ݵĿյ�λͼ����

    //SelectObject(VIDEO_MDC, bmp1);               //����λͼ����ŵ�mdc��
    //GetClientRect(VIDEO_HWND, &VIDEO_RECT);
    ////������Դͼ��λͼ�����
    //VIDEO_BACKGROUND = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_VIDEO_AREA));
    ////��������
    //SelectObject(VIDEO_MDC, hFont);
    //SetBkMode(VIDEO_MDC, TRANSPARENT);    //����������ʾ����͸��
    //VideoDisplay();
    // ���ز��Ű�ť
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


    // ����ͼƬ������
    std::thread ImagePortal_thread(FlashThread, hWnd);
    ImagePortal_thread.detach();

    // ���˵���Դ��ʼ��
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
		// д������
		swprintf_s(str_101, L"��ǰ���ţ�");
		SetTextColor(WIN_MDC, RGB(255, 255, 0));
		TextOut(WIN_MDC, 10, 6, str_101, (int)wcslen(str_101));
		// д�벥��ģʽ start
		swprintf_s(str_101, L"����ģʽ");
		SetTextColor(WIN_MDC, RGB(255, 255, 0));
		TextOut(WIN_MDC, 400, 290, str_101, (int)wcslen(str_101));

		if (0 == play_cycle) {
			swprintf_s(str_101, L"˳�򲥷�");
		}else if (1 == play_cycle) {
			swprintf_s(str_101, L"ѭ������");
		}else if(2 == play_cycle) {
			swprintf_s(str_101, L"����ѭ��");
		}else if(3 == play_cycle) {
			swprintf_s(str_101, L"�������");
		}else if(4 == play_cycle) {
			swprintf_s(str_101, L"��������");
		}		
		SetTextColor(WIN_MDC, RGB(3, 41, 250));
		TextOut(WIN_MDC, 410, 310, str_101, (int)wcslen(str_101));
		// end

		SetTextColor(WIN_MDC, RGB(222, 3, 250));
		TextOut(WIN_MDC, 30, 26, current_play_music_name, (int)wcslen(current_play_music_name));
        //��mdc�е�ȫ����������hdc��
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
        // TODO: �ڴ˴����ʹ�� hdc ���κλ�ͼ����...
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
    case WM_MOUSEMOVE: // ����ƶ���Ϣ
    {
        TRACKMOUSEEVENT tme;
        tme.cbSize = sizeof(TRACKMOUSEEVENT);
        tme.dwFlags = TME_LEAVE | TME_HOVER;
        tme.hwndTrack = hWnd;
        tme.dwHoverTime = 10;
        ::TrackMouseEvent(&tme);
    }
    break;
    case WM_LBUTTONDOWN: // ����������
    {
		if (play_cycle == 3)//�������
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
				lstrcat(current_play, current_play_path);  //���ļ�������·��  
				lstrcat(current_play, current_point);    //�����ļ���  
				lstrcat(current_play_music_name, current_point);
				updateMusicBuff(current_play); 
				current_point += lstrlen(current_point) + 1;     //������һ���ļ�
				current_play_list_index += 1;
			}
		}
		else {
			if (*current_point)
			{
				*current_play = { 0 };
				*current_play_music_name = { 0 };
				lstrcat(current_play, current_play_path);  //���ļ�������·��  
				lstrcat(current_play, current_point);    //�����ļ���  
				lstrcat(current_play_music_name, current_point);
				updateMusicBuff(current_play);
				current_point += lstrlen(current_point) + 1;     //������һ���ļ�
				current_play_list_index += 1;
			}
			else {
				current_point = temp_point;
				*current_play = { 0 };
				*current_play_music_name = { 0 };
				lstrcat(current_play, current_play_path);  //���ļ�������·��  
				lstrcat(current_play, current_point);    //�����ļ���  
				lstrcat(current_play_music_name, current_point);
				updateMusicBuff(current_play);
				current_point += lstrlen(current_point) + 1;     //������һ���ļ�
				current_play_list_index = 0;
			}
			
		}

    }
    break;
    case WM_LBUTTONUP: // ����������
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
        // TODO: �ڴ˴����ʹ�� hdc ���κλ�ͼ����...
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
    case WM_MOUSEMOVE: // ����ƶ���Ϣ
    {
        TRACKMOUSEEVENT tme;
        tme.cbSize = sizeof(TRACKMOUSEEVENT);
        tme.dwFlags = TME_LEAVE | TME_HOVER;
        tme.hwndTrack = hWnd;
        tme.dwHoverTime = 10;
        ::TrackMouseEvent(&tme);
    }
    break;
    case WM_LBUTTONDOWN: // ����������
    {
		if (play_cycle == 3)//�������
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
				lstrcat(current_play, current_play_path);  //���ļ�������·��  
				lstrcat(current_play, current_point);    //�����ļ���  
				lstrcat(current_play_music_name, current_point);
				updateMusicBuff(current_play);
				current_point += lstrlen(current_point) + 1;     //������һ���ļ�
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
				lstrcat(current_play, current_play_path);  //���ļ�������·��  
				lstrcat(current_play, current_point);    //�����ļ���  
				lstrcat(current_play_music_name, current_point);
				updateMusicBuff(current_play);
				current_point += lstrlen(current_point) + 1;     //������һ���ļ�
				if (*current_point) {
				}
				else {
					current_point = temp_point;
				}
			}
		}


    }
    break;
    case WM_LBUTTONUP: // ����������
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
        // TODO: �ڴ˴����ʹ�� hdc ���κλ�ͼ����...
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
    case WM_MOUSEMOVE: // ����ƶ���Ϣ
    {
        TRACKMOUSEEVENT tme;
        tme.cbSize = sizeof(TRACKMOUSEEVENT);
        tme.dwFlags = TME_LEAVE | TME_HOVER;
        tme.hwndTrack = hWnd;
        tme.dwHoverTime = 10;
        ::TrackMouseEvent(&tme);
    }
    break;
    case WM_LBUTTONDOWN: // ����������
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
    case WM_LBUTTONUP: // ����������
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
    SetCursorPos(screen_point.x + 1, screen_point.y + 1);//����ָ������
    SetCursorPos(screen_point.x, screen_point.y);//����ָ������
}
void pauseMusic(HWND hWnd) {    
    getPlayStatus(hWnd);
    if (!local_str_cm(STATUS_PAUSED, music_play_status)) {
        mciSendString(PAUSE_SONG, NULL, 0, NULL);
        btn_play->display();
        GetCursorPos(&screen_point);
        SetCursorPos(screen_point.x + 1, screen_point.y + 1);//����ָ������
        SetCursorPos(screen_point.x, screen_point.y);//����ָ������
    }
}
void playMusic(HWND hWnd)
{
    if (video_play_flag) { // ֹͣ��Ƶ����

    }
    if (music_play_new) {
        if(!local_str_cm(L"", music_buff)){
            mciSendString(STOP_SONG, NULL, 0, NULL);
            wchar_t buff[256] = { 0 };
            mciSendString(music_buff, buff, 256, NULL);
            //MessageBox(hWnd, buff, L"����", MB_OK); // ���ض�ý��Ľ�� 1��normal
            mciSendString(PLAY_SONG, buff, 256, WIN_HWND);
            getPlayStatus(hWnd);
            // ��ȡ��������
            wchar_t sPosition[256] = { 0 };
            mciSendString(SONG_LENGTH, sPosition, 255, 0);//USES_CONVERSION;  A2W()
            song_length = strtol(tchar2cchat(sPosition), NULL, 10);
            // ��ȡ����
            wchar_t chVolume[256];
            mciSendString(SONG_VOLUME, chVolume, 255, 0);
            song_volume = strtol(tchar2cchat(chVolume), NULL, 10);
            music_play_new = false;
            music_play_done = false;
            btn_pause->display();
            GetCursorPos(&screen_point);
            SetCursorPos(screen_point.x + 1, screen_point.y + 1);//����ָ������
            SetCursorPos(screen_point.x, screen_point.y);//����ָ������
        }
        else {
            music_play_flag = false;
            MessageBox(hWnd, L"��ǰû�д��κ��ļ��򲥷��б�!", L"����ʧ��", MB_OK);
        }
    }else
    // ���״̬
    {
        getPlayStatus(hWnd);
        if (local_str_cm(STATUS_PAUSED, music_play_status)) {
            mciSendString(RESUME_SONG, NULL, 0, WIN_HWND);
            btn_pause->display();
            GetCursorPos(&screen_point);
            SetCursorPos(screen_point.x + 1, screen_point.y + 1);//����ָ������
            SetCursorPos(screen_point.x, screen_point.y);//����ָ������
        }
        else if (local_str_cm(STATUS_STOPPED, music_play_status)) {            
            music_play_done = true;
            music_play_new = true;
            
			if (0 == play_cycle) { //// 0��˳�򲥷�
				if (*current_point)
				{
					*current_play = { 0 };
					*current_play_music_name = { 0 };
					lstrcat(current_play, current_play_path);  //���ļ�������·��  
					lstrcat(current_play, current_point);    //�����ļ���  
					lstrcat(current_play_music_name, current_point);
					updateMusicBuff(current_play);

					current_point += lstrlen(current_point) + 1;     //������һ���ļ�
					current_play_list_index += 1;
				}
				else {
					current_point = temp_point;

					*current_play = { 0 };
					*current_play_music_name = { 0 };
					lstrcat(current_play, current_play_path);  //���ļ�������·��  
					lstrcat(current_play, current_point);    //�����ļ���  
					lstrcat(current_play_music_name, current_point);
					updateMusicBuff(current_play);

					current_point += lstrlen(current_point) + 1;     //������һ���ļ�
					current_play_list_index = 0;

					music_play_flag = false;
					setStop();
				}
			}
			else if (1 == play_cycle) {//1��ѭ������
				if (*current_point && current_play_list_length > 1)// && *next_point)
				{
					*current_play = { 0 };
					*current_play_music_name = { 0 };
					lstrcat(current_play, current_play_path);  //���ļ�������·��  
					lstrcat(current_play, current_point);    //�����ļ���  
					lstrcat(current_play_music_name, current_point);
					updateMusicBuff(current_play);

					current_point += lstrlen(current_point) + 1;     //������һ���ļ�
					current_play_list_index += 1;
				}
				else {
					current_point = temp_point;

					*current_play = { 0 };
					*current_play_music_name = { 0 };
					lstrcat(current_play, current_play_path);  //���ļ�������·��  
					lstrcat(current_play, current_point);    //�����ļ���  
					lstrcat(current_play_music_name, current_point);
					updateMusicBuff(current_play);

					current_point += lstrlen(current_point) + 1;     //������һ���ļ�
					current_play_list_index = 0;
				}
			}
			else if (2 == play_cycle) {//2������ѭ��
			}
			else if (3 == play_cycle) {//3���������
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
					lstrcat(current_play, current_play_path);  //���ļ�������·��  
					lstrcat(current_play, current_point);    //�����ļ���  
					lstrcat(current_play_music_name, current_point);
					updateMusicBuff(current_play);
					current_point += lstrlen(current_point) + 1;     //������һ���ļ�
					current_play_list_index += 1;
				}
			}
			else {
				music_play_flag = false;
				setStop();
			}
        }   
    }

    
    //MessageBox(hWnd, buff, L"����", MB_OK);

    //PlaySound(TEXT("D:\example.wma"), hInstance, SND_FILENAME | SND_ASYNC | SND_LOOP); //ѭ�����ű�������
    //PlaySound(MAKEINTRESOURCE(IDR_WAVE1), hInstance, SND_RESOURCE | SND_ASYNC | SND_LOOP); //ѭ�����ű�������
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
	// strcmp ����wcscmp����������ַ����ǿ��ַ�����Ҫ�ÿ��ַ����ĺ������Ƚϡ�
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
    ofn.lpstrFilter = _T("����֧������(*.mp3;*.wma;*.m4a;*.flac)\0*.mp3;*.wma;*.m4a;*.flac\0"
        "MP3(*.mp3)\0*.mp3\0"
        "WMA(*.wma)\0*.wma\0"
        "M4A(*.m4a)\0*.m4a\0"
        "FLAC(*.flac)\0*.flac\0"
        "����֧����Ƶ(*.mp4;*.mkv;*.rmvb)\0*.mp4;*.mkv;*.rmvb\0"
        "MP4(*.mp4)\0*.mp4\0"
        "MKV(*.mkv)\0*.mkv\0"
        "RMVB(*.rmvb)\0*.rmvb\0"
        "�����ļ�(*.*)\0*.*\0");//Ҫѡ����ļ���׺   
    ofn.lpstrInitialDir = _T("D:\\");//Ĭ�ϵ��ļ�·��
    ofn.lpstrFile = current_play;//����ļ��Ļ�����   
    ofn.nMaxFile = sizeof(current_play) / sizeof(*current_play);
    ofn.nFilterIndex = 0;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;//��־����Ƕ�ѡҪ����OFN_ALLOWMULTISELECT  
    //BOOL bSel = GetOpenFileName(&ofn);
    if (!GetOpenFileName(&ofn)) { /*MessageBox(hWnd, L"�ļ���ʧ�ܣ�", L"����", MB_OK);*/ return false; }
    else {
		*current_play_music_name = { 0 };
		lstrcpyn(current_play_path, current_play, ofn.nFileOffset);//lstrcpyn(szPath, szOpenFileNames, ofn.nFileOffset);

		current_play_path[ofn.nFileOffset] = '\0';
		int nLen = 0;
		nLen = lstrlen(current_play_path);

		if (current_play_path[nLen - 1] != '\\')   //���ѡ�˶���ļ�,��������'//'
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
    ofn.lpstrFilter = _T("����֧��������Ƶ(*.mp3;*.wma;*.m4a;*.flac;*.mp4;*.mkv;*.rmvb)\0*.mp3;*.wma;*.m4a;*.flac;*.mp4;*.mkv;*.rmvb\0"
        "MP3(*.mp3)\0*.mp3\0"
        "WMA(*.wma)\0*.wma\0"
        "M4A(*.m4a)\0*.m4a\0"
        "FLAC(*.flac)\0*.flac\0"
        "MP4(*.mp4)\0*.mp4\0"
        "MKV(*.mkv)\0*.mkv\0"
        "RMVB(*.rmvb)\0*.rmvb\0");
    ofn.lpstrInitialDir = _T("D:\\");//Ĭ�ϵ��ļ�·��
    ofn.nFilterIndex = 0;
    if (GetOpenFileName(&ofn))
    {
		current_play_list_length = 0;
        //�ѵ�һ���ļ���ǰ�ĸ��Ƶ�szPath,��:
        //���ֻѡ��һ���ļ�,�͸��Ƶ����һ��'/'
        //���ѡ�˶���ļ�,�͸��Ƶ���һ��NULL�ַ�
        lstrcpyn(current_play_path, current_play_list, ofn.nFileOffset);//lstrcpyn(szPath, szOpenFileNames, ofn.nFileOffset);
        //��ֻѡ��һ���ļ�ʱ,�������NULL�ַ��Ǳ����.
        //���ﲻ����Դ�ѡ��һ���Ͷ���ļ������
        /*szPath[ofn.nFileOffset] = '\0';
        nLen = lstrlen(szPath);*/
		current_play_path[ofn.nFileOffset] = '\0';
        nLen = lstrlen(current_play_path);

        if (current_play_path[nLen - 1] != '\\')   //���ѡ�˶���ļ�,��������'//'
        {
            lstrcat(current_play_path, TEXT("\\"));
        }

        //p = szOpenFileNames + ofn.nFileOffset; //��ָ���Ƶ���һ���ļ�
		current_point = current_play_list + ofn.nFileOffset;
		temp_point = current_point;

		if(*current_point){
			*current_play = { 0 };
			*current_play_music_name = { 0 };
			lstrcat(current_play, current_play_path);  //���ļ�������·��  
			lstrcat(current_play, current_point);    //�����ļ��� 
			lstrcat(current_play_music_name,current_point);
			updateMusicBuff(current_play);
			music_play_flag = true;
			start_auto_play_thread();

			current_point += lstrlen(current_point) + 1;     //������һ���ļ�
			current_play_list_index = 0;
		}
        //ZeroMemory(current_play_list, sizeof(current_play_list));
		TCHAR* p;
		p = temp_point;
        while (*p)
        {
			current_play_list_length += 1;
            p += lstrlen(p) + 1;     //������һ���ļ�
        }
		if (4 == play_cycle) {
			play_cycle = 0;
		}
        //MessageBox(hWnd, szFileName, L"MultiSelect", MB_OK);
    }
    else {
        //MessageBox(hWnd, TEXT("��ѡ���ʧ�ܣ�"), TEXT("MultiSelect"), MB_OK);
    }
}
void ChoicePlayPath(HWND hWnd)
{
    TCHAR path[MAX_PATH] = { 0 };
    BROWSEINFO bi;
    ZeroMemory(&bi, sizeof(BROWSEINFO));
    bi.hwndOwner = hWnd;
    bi.pszDisplayName = path;
    bi.lpszTitle = L"��ѡ�񲥷�·��";
    //bi.pidlRoot = _T("D:\\");
    bi.ulFlags = BIF_RETURNONLYFSDIRS;// BIF_BROWSEINCLUDEFILES
    LPITEMIDLIST idl = SHBrowseForFolder(&bi);
    if (NULL != idl) {
        SHGetPathFromIDList(idl, path);
        if (local_str_cm(path, L""))
        {
            MessageBox(hWnd, TEXT("��ѡ���ʧ�ܣ�"), TEXT("PathSelect"), MB_OK);
        }
        //MessageBox(hWnd, path, TEXT("��ѡ��"), MB_OK);
    }
    else {
        //MessageBox(hWnd, TEXT("��ѡ���ʧ�ܣ�"), TEXT("PathSelect"), MB_OK);
    }
}
// 2021-12-26 end
//
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��: ���������ڵ���Ϣ��
//
//  WM_COMMAND  - ����Ӧ�ó���˵�
//  WM_PAINT    - ����������
//  WM_DESTROY  - �����˳���Ϣ������
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // �����˵�ѡ��:
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
		case IDM_CYCLE_CLOSED://0��˳�򲥷�   
		{
			play_cycle = 0;
			cycle = false;
			random_play = false;
		}break;
		case IDM_CYCLE_LIST://1��ѭ������
		{
			play_cycle = 1;
			cycle = true;
			random_play = false;
		}break;
		case IDM_CYCLE_SIGNAL://2������ѭ��
		{
			play_cycle = 2;
			cycle = false;
			random_play = false;
		}break;
		case IDM_CYCLE_RANDOM://3���������
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
        // TODO: �ڴ˴����ʹ�� hdc ���κλ�ͼ����...
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_NCLBUTTONDOWN:
    {
        //MessageBox(hWnd, L"�ǿͻ�����", L"����", MB_OK);
        DefWindowProc(hWnd, message, wParam, lParam);
    }
        break;
    case WM_MOUSEMOVE: // ����ƶ���Ϣ
    {
        MOUSEX = LOWORD(lParam);
        MOUSEY = HIWORD(lParam);
        //GetWindowRect(hWnd, &WINDOW_RECT);
        //GetClientRect(hWnd, &WIN_RECT);
        //wsprintf(buf, L"WINDOW_RECT left %d right %d bottom %d top %d ", WINDOW_RECT.left, WINDOW_RECT.right, WINDOW_RECT.bottom, WINDOW_RECT.top);
        //MessageBox(hWnd, buf, L"��������", MB_OK);
        //wsprintf(buf, L"WIN_RECT left %d right %d bottom %d top %d ", WIN_RECT.left, WIN_RECT.right, WIN_RECT.bottom, WIN_RECT.top);
        //MessageBox(hWnd, buf, L"�ͻ���", MB_OK);
    }
    break;
    case WM_SETCURSOR:
    {
        GetCursorPos(&screen_point);
        //SetCursorPos(screen_point.x + 1, screen_point.y + 10);//����ָ������
        ScreenToClient(hWnd, &screen_point);
        //wsprintf(buf, L"WIN_RECT x %d y %d ", screen_point.x, screen_point.y);
        //MessageBox(hWnd, buf, L"�ͻ���", MB_OK);
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
        // TODO: �ڴ˴����ʹ�� hdc ���κλ�ͼ����...
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

// �����ڡ������Ϣ�������
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