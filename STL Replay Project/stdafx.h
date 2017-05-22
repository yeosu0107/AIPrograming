// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �Ǵ� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#include <windows.h>

// C ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
#include "Replay.h"
#include "GamePlay.h"



struct grid
{
	int statue;
	int num;
	bool mine;
	bool flag;
	bool blank;
	bool error;
	bool temp;
	bool boom;
	bool mouse;
};

#define up 1
#define down 0

enum gameType { nowPlay = 0, nowReplay, checkScene };

extern Replay	replay;
extern grid		map[30][16];
extern HWND	slotHwnd;
extern HINSTANCE g_hInst;
extern RECT rt;


extern int x[8];
extern int y[8];
extern int Xnum, Ynum, mine;

extern chrono::time_point<chrono::system_clock> tp;

extern time_t start;
extern time_t now;

extern INT_PTR CALLBACK SlotProc(HWND, UINT, WPARAM, LPARAM);