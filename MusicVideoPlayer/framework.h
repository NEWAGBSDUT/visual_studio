#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // �� Windows ͷ�ļ����ų�����ʹ�õ�����

// ͼƬ�ؼ�
//#define _AFXDLL
//#include <afxwin.h>

// Windows ͷ�ļ�
#include <windows.h>
// C ����ʱͷ�ļ�
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// 
//#include <exception>
//#include <thread>
// �򿪶Ի���ѡ�񵥸��ļ�
#include <commdlg.h>
// ���ļ��� ���� 2021-12-26
#include <ShlObj.h>

// ��������
#include <mmsystem.h>
#pragma comment(lib, "WINMM.LIB")
// ͼƬ������
#pragma comment(lib,"Msimg32.lib")//���ʹ��TransparentBlt��������Ŀ��ļ�

//�Զ�����
#include "ImgButton.h"
#include "Flash.h"

// tooltips
//#include <windowsx.h>
#include <CommCtrl.h>
#pragma comment(lib, "comctl32.lib")

//#include <iostream>
//#include <string>
//using namespace std;
