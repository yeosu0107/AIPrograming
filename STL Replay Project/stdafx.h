// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
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