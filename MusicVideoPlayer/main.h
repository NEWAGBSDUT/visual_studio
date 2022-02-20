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
/// 主窗口变量
/// </summary>
RECT WIN_RECT;  // 定义一个RECT结构体，用于储存内部窗口区域的坐标,即程序窗口大小 
HWND WIN_HWND;  // 主窗口句柄
HDC  WIN_HDC;
HDC  WIN_MDC;
HDC  WIN_BUFDC;
#define WIN_WIDTH      520
#define WIN_HEIGHT     400
int WIN_MARGIN_WIDTH = 200;
int WIN_MARGIN_HEIGHT = 80;

/// ///////////////////////////////////
//   鼠标
POINT                                 screen_point;
int                                   MOUSEX = 0; // 鼠标x坐标
int                                   MOUSEY = 0; // 鼠标y坐标

///////////////////////////////////////////
// 视频窗口区域
//
HWND    VIDEO_HWND; // 视频窗口句柄
#define VIDEO_CLASS_NAME "VideoArea"
#define VIDEO_WIDTH      520
#define VIDEO_HEIGHT     280
#define VIDEO_MARGIN_WIDTH      0
#define VIDEO_MARGIN_HEIGHT     0
HDC     VIDEO_HDC;  // 视频窗口---HDC
HDC     VIDEO_MDC; // 和视频窗口HDC兼容的内存DC
HDC     VIDEO_BUFDC; // 和视频窗口HDC兼容的缓冲DC
RECT    VIDEO_RECT; //定义一个RECT结构体，用于储存视频窗口区域的坐标
HBITMAP VIDEO_BACKGROUND;

//////////////////////////////////////////////
/// </summary> 线程开关
BOOL                           FLASHFLAG = TRUE; // 动画线程开关
#define                            FLASH_SLEEP 40
BOOL          AUTOPLAY  = TRUE; // 自动播放线程
#define       AUTO_PLAY_SLEEP 300
bool          exists_auto_play_thread = false;
///////////////////////////////////////////
// 歌曲 视频 共享
// 
bool current_choice_song = true; // 默认选择歌曲
TCHAR current_play_path[MAX_PATH] = { 0 }; // 当前播放路径
TCHAR current_play_list[100 * MAX_PATH] = { 0 }; // 当前播放列表
TCHAR current_play[MAX_PATH] = { 0 }; // 当前播放
int      current_play_list_length = 0;
int    current_play_list_index = 0;
TCHAR* current_point = NULL;
TCHAR* temp_point = NULL; // 缓存指针总是指向 列表开头
bool cycle = false; // 循环播放
bool random_play = false; // 随机播放
wchar_t str_101[64];
TCHAR current_play_music_name[MAX_PATH] = { 0 }; // 当前播放

int play_cycle = 0;// 0、顺序播放 1、循环播放 2、单曲循环 3、随机播放 4、单曲播放
///////////////////////////////////////////
// 音乐播放
#define BTN_MARGIN_Y 289
// 下一首按钮
int next_imgs[] = { IDB_NEXT, IDB_NEXT_HOVER };
ImageButton* btn_next = new ImageButton("btn_next", "next",
	36, 36, next_imgs);
#define BTN_NEXT_X 300
#define BTN_NEXT_Y BTN_MARGIN_Y
// 上一首按钮
int previous_imgs[] = { IDB_PREVIOUS, IDB_PREVIOUS_HOVER };
ImageButton* btn_previous = new ImageButton("btn_previous", "previous",
	36, 36, previous_imgs);
#define BTN_PREVIOUS_X 120
#define BTN_PREVIOUS_Y BTN_MARGIN_Y
// 播放按钮
int play_imgs[] = { IDB_PLAY, IDB_PLAY_HOVER };
ImageButton* btn_play = new ImageButton("btn_play", "play",
	36, 36, play_imgs);
#define BTN_PLAY_X 210
#define BTN_PLAY_Y BTN_MARGIN_Y
// 暂停按钮
int pause_imgs[] = { IDB_PAUSE, IDB_PAUSE_HOVER };
ImageButton* btn_pause = new ImageButton("btn_pause", "pause",
	36, 36, pause_imgs);
//#define BTN_PAUSE_X 210
//#define BTN_PAUSE_Y BTN_MARGIN_Y
//---------------
wchar_t music_buff[256]; // 格式化音乐字符串,当前歌曲
HBITMAP MUSIC_PLAY_DIALOG; // 默认音乐播放背景图片
//2021-12-23 start
bool    music_play_flag = false; // 是否在播放音乐
bool    music_play_new  = true; // 是否重新播放音乐
bool    music_play_done = true; // 是否播放完毕
#define SONG_ALIAS     TEXT("my_music") // 歌曲别名
#define PAUSE_SONG      TEXT("pause my_music")              // 暂停歌曲
#define PLAY_SONG      TEXT("play my_music")              // 播放歌曲
#define RESUME_SONG      TEXT("resume my_music")              // 恢复播放歌曲
#define STOP_SONG      TEXT("close my_music")              // 停止播放
#define REPEAT_SONG    TEXT("play my_music repeat")     // 重复播放歌曲
#define SONG_LENGTH    TEXT("status my_music length")     // 获取歌曲长度
#define SONG_VOLUME   TEXT("status my_music volume")     // 获取歌曲播放音量
wchar_t seek_pos_song[] = L"seek my_music to %ld";  // 跳转到指定位置播放歌曲
#define SONG_MODE     TEXT("status my_music mode")

#define STATUS_PAUSED  TEXT("paused")
#define STATUS_STOPPED TEXT("stopped")
//#define STATUS_CLOSED  TEXT("closed")
#define STATUS_PLAYING TEXT("playing")

wchar_t music_play_status[256] = { 0 };
long    song_length = 0;//当前播放歌曲长度 长度单位是毫秒
long    song_volume; //当前播放歌曲的声音大小
#define MP3 TEXT(".MP3")
#define WMA TEXT(".WMA")
#define M4A TEXT(".M4A")
#define FLAC TEXT(".FLAC")


/////////////////////////////////
// 视频
// 
bool video_play_flag = false; // 当前是否在播放视频
#define MP4 TEXT(".MP4")
#define MKV TEXT(".MKV")
#define RMVB TEXT(".RMVB")
//2021-12-23 END