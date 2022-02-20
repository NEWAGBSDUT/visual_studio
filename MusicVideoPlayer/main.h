#pragma once
#include "resource.h"

#if(_WIN32_WINNT >= 0x0601)
#define WDA_EXCLUDEFROMCAPTURE 0x00000011
#endif
//#ifdef UNICODE
//#define GetModuleFileName  GetModuleFileNameW
//#else
//#define GetModuleFileName  GetModuleFileNameA
//#endif // !UNICOD



/// <summary>
/// �����ڱ���
/// </summary>
RECT WIN_RECT;  // ����һ��RECT�ṹ�壬���ڴ����ڲ��������������,�����򴰿ڴ�С 
HWND WIN_HWND;  // �����ھ��
HDC  WIN_HDC;
HDC  WIN_MDC;
HDC  WIN_BUFDC;
#define WIN_WIDTH      520
#define WIN_HEIGHT     400
int WIN_MARGIN_WIDTH = 200;
int WIN_MARGIN_HEIGHT = 80;

/// ///////////////////////////////////
//   ���
POINT                                 screen_point;
int                                   MOUSEX = 0; // ���x����
int                                   MOUSEY = 0; // ���y����

///////////////////////////////////////////
// ��Ƶ��������
//
HWND    VIDEO_HWND; // ��Ƶ���ھ��
#define VIDEO_CLASS_NAME "VideoArea"
#define VIDEO_WIDTH      520
#define VIDEO_HEIGHT     280
#define VIDEO_MARGIN_WIDTH      0
#define VIDEO_MARGIN_HEIGHT     0
HDC     VIDEO_HDC;  // ��Ƶ����---HDC
HDC     VIDEO_MDC; // ����Ƶ����HDC���ݵ��ڴ�DC
HDC     VIDEO_BUFDC; // ����Ƶ����HDC���ݵĻ���DC
RECT    VIDEO_RECT; //����һ��RECT�ṹ�壬���ڴ�����Ƶ�������������
HBITMAP VIDEO_BACKGROUND;

//////////////////////////////////////////////
/// </summary> �߳̿���
BOOL                           FLASHFLAG = TRUE; // �����߳̿���
#define                            FLASH_SLEEP 40
BOOL          AUTOPLAY  = TRUE; // �Զ������߳�
#define       AUTO_PLAY_SLEEP 300
bool          exists_auto_play_thread = false;
///////////////////////////////////////////
// ���� ��Ƶ ����
// 
bool current_choice_song = true; // Ĭ��ѡ�����
TCHAR current_play_path[MAX_PATH] = { 0 }; // ��ǰ����·��
TCHAR current_play_list[100 * MAX_PATH] = { 0 }; // ��ǰ�����б�
TCHAR current_play[MAX_PATH] = { 0 }; // ��ǰ����
int      current_play_list_length = 0;
int    current_play_list_index = 0;
TCHAR* current_point = NULL;
TCHAR* temp_point = NULL; // ����ָ������ָ�� �б�ͷ
bool cycle = false; // ѭ������
bool random_play = false; // �������
wchar_t str_101[64];
TCHAR current_play_music_name[MAX_PATH] = { 0 }; // ��ǰ����

int play_cycle = 0;// 0��˳�򲥷� 1��ѭ������ 2������ѭ�� 3��������� 4����������
///////////////////////////////////////////
// ���ֲ���
#define BTN_MARGIN_Y 289
// ��һ�װ�ť
int next_imgs[] = { IDB_NEXT, IDB_NEXT_HOVER };
ImageButton* btn_next = new ImageButton("btn_next", "next",
	36, 36, next_imgs);
#define BTN_NEXT_X 300
#define BTN_NEXT_Y BTN_MARGIN_Y
// ��һ�װ�ť
int previous_imgs[] = { IDB_PREVIOUS, IDB_PREVIOUS_HOVER };
ImageButton* btn_previous = new ImageButton("btn_previous", "previous",
	36, 36, previous_imgs);
#define BTN_PREVIOUS_X 120
#define BTN_PREVIOUS_Y BTN_MARGIN_Y
// ���Ű�ť
int play_imgs[] = { IDB_PLAY, IDB_PLAY_HOVER };
ImageButton* btn_play = new ImageButton("btn_play", "play",
	36, 36, play_imgs);
#define BTN_PLAY_X 210
#define BTN_PLAY_Y BTN_MARGIN_Y
// ��ͣ��ť
int pause_imgs[] = { IDB_PAUSE, IDB_PAUSE_HOVER };
ImageButton* btn_pause = new ImageButton("btn_pause", "pause",
	36, 36, pause_imgs);
//#define BTN_PAUSE_X 210
//#define BTN_PAUSE_Y BTN_MARGIN_Y
//---------------
wchar_t music_buff[256]; // ��ʽ�������ַ���,��ǰ����
HBITMAP MUSIC_PLAY_DIALOG; // Ĭ�����ֲ��ű���ͼƬ
//2021-12-23 start
bool    music_play_flag = false; // �Ƿ��ڲ�������
bool    music_play_new  = true; // �Ƿ����²�������
bool    music_play_done = true; // �Ƿ񲥷����
#define SONG_ALIAS     TEXT("my_music") // ��������
#define PAUSE_SONG      TEXT("pause my_music")              // ��ͣ����
#define PLAY_SONG      TEXT("play my_music")              // ���Ÿ���
#define RESUME_SONG      TEXT("resume my_music")              // �ָ����Ÿ���
#define STOP_SONG      TEXT("close my_music")              // ֹͣ����
#define REPEAT_SONG    TEXT("play my_music repeat")     // �ظ����Ÿ���
#define SONG_LENGTH    TEXT("status my_music length")     // ��ȡ��������
#define SONG_VOLUME   TEXT("status my_music volume")     // ��ȡ������������
wchar_t seek_pos_song[] = L"seek my_music to %ld";  // ��ת��ָ��λ�ò��Ÿ���
#define SONG_MODE     TEXT("status my_music mode")

#define STATUS_PAUSED  TEXT("paused")
#define STATUS_STOPPED TEXT("stopped")
//#define STATUS_CLOSED  TEXT("closed")
#define STATUS_PLAYING TEXT("playing")

wchar_t music_play_status[256] = { 0 };
long    song_length = 0;//��ǰ���Ÿ������� ���ȵ�λ�Ǻ���
long    song_volume; //��ǰ���Ÿ�����������С
#define MP3 TEXT(".MP3")
#define WMA TEXT(".WMA")
#define M4A TEXT(".M4A")
#define FLAC TEXT(".FLAC")


/////////////////////////////////
// ��Ƶ
// 
bool video_play_flag = false; // ��ǰ�Ƿ��ڲ�����Ƶ
#define MP4 TEXT(".MP4")
#define MKV TEXT(".MKV")
#define RMVB TEXT(".RMVB")
//2021-12-23 END