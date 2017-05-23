#include "stdafx.h"
//#include "GamePlay.h"
#include "resource.h"
void CreateMap()
{
	int mine_num = mine;
	srand(time(NULL));
	while (1)
	{
		for (int i = 0; i<Ynum; i++) {
			for (int j = 0; j<Xnum; j++)
			{
				if (map[i][j].mine == false && rand() % 6 == 2) {
					map[i][j].mine = true;
					replay.saveMine(j, i);
					mine_num--;
					if (mine_num == 0)
						break;
				}
				if (mine_num == 0)
					break;
			}
			if (mine_num == 0)
				break;
		}
		if (mine_num == 0)
			break;
	}
	for (int i = 0; i<Ynum; i++)
	{
		for (int j = 0; j<Xnum; j++)
		{
			for (int q = 0; q<8; q++)
			{
				if (i + y[q] >= 0 && i + y[q] <= Ynum && j + x[q] >= 0 && j + x[q] <= Xnum)
					if (map[i][j].mine == false && map[i + y[q]][j + x[q]].mine == true)
						map[i][j].num++;

			}
			if (map[i][j].num == 0 && map[i][j].mine == false)
				map[i][j].blank = true;
		}
	}
}
void CheckBlank(int i, int j)
{

	for (int p = -1; p <= 1; p++)
	{
		for (int q = -1; q <= 1; q++)
		{
			if (i + p<0 || i + p >= Ynum || j + q<0 || j + q >= Xnum)
				continue;
			if (map[i + p][j + q].num != 0 && map[i + p][j + q].statue == up) {
				map[i + p][j + q].statue = down;
				continue;
			}
			else if (map[i + p][j + q].statue == up && map[i + p][j + q].blank == true) {
				map[i + p][j + q].statue = down;
				CheckBlank(i + p, j + q);
			}

		}
	}
}
void gameover(HWND hwnd, int select, int type)
{
	KillTimer(hwnd, 2);
	KillTimer(hwnd, 3);
	for (int i = 0; i<Ynum; i++) {
		for (int j = 0; j<Xnum; j++) {
			if (map[i][j].mine == false && map[i][j].flag == true)
				map[i][j].error = true;
			map[i][j].statue = down;
		}
	}
	switch (select)
	{
	case 1:
		if (type == gameType::nowReplay) {
			MessageBox(hwnd, L"패배\n지뢰를 밟았습니다.", L"리플레이 종료", MB_OK);
			return;
		}
		if (MessageBox(hwnd, L"리플레이를\n저장하시겠습니까?", L"패배 - 지뢰를 밟았습니다.", MB_YESNO) == IDYES) {
			if (!IsWindow(slotHwnd)) {
				slotHwnd = CreateDialog(g_hInst, MAKEINTRESOURCE(IDD_DIALOG1),
					hwnd, SlotProc);
				ShowWindow(slotHwnd, SW_SHOW);
			}
		}
		break;
	case 2:
		if (type == gameType::nowReplay) {
			MessageBox(hwnd, L"패배\n잘못된 곳에 지뢰를 선택", L"리플레이 종료", MB_OK);
			return;
		}
		if (MessageBox(hwnd, L"리플레이를\n저장하시겠습니까?", L"패배 - 잘못된 곳에 지뢰를 선택", MB_YESNO) == IDYES) {
			if (!IsWindow(slotHwnd)) {
				slotHwnd = CreateDialog(g_hInst, MAKEINTRESOURCE(IDD_DIALOG1),
					hwnd, SlotProc);
				ShowWindow(slotHwnd, SW_SHOW);
			}
		}
		break;
	}

}
void result(HWND hwnd, int type)
{
	int select = 0;
	KillTimer(hwnd, 2);
	KillTimer(hwnd, 3);
	for (int i = 0; i<Ynum; i++) {
		for (int j = 0; j<Xnum; j++) {
			if (map[i][j].flag == true && map[i][j].mine == false) {
				select = 1;
				break;
			}
			if (map[i][j].boom == true) {
				select = 2;
				break;
			}
		}
		if (select == 1 || select == 2)
			break;
	}

	switch (select)
	{
	case 1:
		if (type == gameType::nowReplay) {
			MessageBox(hwnd, L"패배\n잘못된 곳에 지뢰를 선택", L"리플레이 종료", MB_OK);
			return;
		}
		if (MessageBox(hwnd, L"리플레이를\n저장하시겠습니까?", L"패배 - 잘못된 곳에 지뢰를 선택", MB_YESNO) == IDYES) {
			if (!IsWindow(slotHwnd)) {
				slotHwnd = CreateDialog(g_hInst, MAKEINTRESOURCE(IDD_DIALOG1),
					hwnd, SlotProc);
				ShowWindow(slotHwnd, SW_SHOW);
			}
		}
		break;
	case 0:
		if (type == gameType::nowReplay) {
			MessageBox(hwnd, L"승리하셨습니다.", L"리플레이 종료", MB_OK);
			return;
		}
		if (MessageBox(hwnd, L"리플레이를\n저장하시겠습니까?", L"승리!", MB_YESNO)) {
			if (!IsWindow(slotHwnd)) {
				slotHwnd = CreateDialog(g_hInst, MAKEINTRESOURCE(IDD_DIALOG1),
					hwnd, SlotProc);
				ShowWindow(slotHwnd, SW_SHOW);
			}
		}
		break;
	case 2:
		if (type == gameType::nowReplay) {
			MessageBox(hwnd, L"패배\n지뢰를 밟았습니다.", L"리플레이 종료", MB_OK);
			return;
		}
		if (MessageBox(hwnd, L"리플레이를\n저장하시겠습니까?", L"패배 - 지뢰를 밟았습니다.", MB_YESNO) == IDYES) {
			if (!IsWindow(slotHwnd)) {
				slotHwnd = CreateDialog(g_hInst, MAKEINTRESOURCE(IDD_DIALOG1),
					hwnd, SlotProc);
				ShowWindow(slotHwnd, SW_SHOW);
			}
		}
		break;
	}

}
void SetNewMap(HWND hwnd)
{
	for (int i = 0; i<30; i++)
		for (int j = 0; j<16; j++) {
			map[i][j].statue = up;
			map[i][j].mine = false;
			map[i][j].num = 0;
			map[i][j].flag = false;
			map[i][j].blank = false;
			map[i][j].error = false;
			map[i][j].temp = false;
			map[i][j].boom = false;
		}

	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);
	SetWindowPos(hwnd, HWND_TOP, 100, 100, rt.right - rt.left, rt.bottom - rt.top, NULL);
}


void ClickBlank(HWND& hwnd, const int& xPos, const int& yPos, const int& type) {
	replay.input(click::left, xPos, yPos);
	map[yPos][xPos].statue = down;
	if (map[yPos][xPos].mine == true) {
		map[yPos][xPos].boom = true;
		gameover(hwnd, 1, type);
		return;
	}
	if (map[yPos][xPos].blank == true)
		CheckBlank(yPos, xPos);
}
void ClickLR(HWND& hwnd, const int& xPos, const int& yPos, int& t_num, int& f_num, int type) {
	int j = xPos;
	int i = yPos;
	replay.input(click::both, j, i);
	t_num = map[i][j].num;
	for (int q = 0; q < 8; q++)
	{
		if (map[i + y[q]][j + x[q]].flag == true)
			f_num++;
		if (map[i + y[q]][j + x[q]].mine == false && map[i + y[q]][j + x[q]].flag == true) {
			gameover(hwnd, 2, type);
			return;
		}

		else {
			if (i + y[q] < 0 || i + y[q] >= Ynum || j + x[q] < 0 || j + x[q] >= Xnum)
				continue;
			if (map[i + y[q]][j + x[q]].statue == up)
				map[i + y[q]][j + x[q]].temp = true;
		}
	}
	if (f_num == t_num)
	{
		CheckBlank(i, j);
		for (int q = 0; q < 8; q++)
		{
			if (map[i + y[q]][j + x[q]].temp == true)
				map[i + y[q]][j + x[q]].statue = down;
		}
	}
}
void UpLR() {
	for (int i = 0; i < Ynum; i++) {
		for (int j = 0; j < Xnum; j++) {
			map[i][j].temp = false;
		}
	}
}
void ClickFlag(const int& xPos, const int& yPos, int& mine_num) {
	int j = xPos;
	int i = yPos;
	replay.input(click::right, j, i);
	if (map[i][j].statue == up) {
		map[i][j].statue = down;
		map[i][j].flag = true;
		mine_num--;
	}
	else if (map[i][j].statue == down && map[i][j].flag == true) {
		map[i][j].statue = up;
		map[i][j].flag = false;
		mine_num++;
	}
}
void SetLevel(int level, HWND& hwnd, int& mine_num, int type) {
	switch (level)
	{
	case 1:
		rt.top = 0;
		rt.left = 0;
		rt.bottom = 340;
		rt.right = 280;

		Xnum = 9; Ynum = 9;
		mine_num = mine = 10;
		break;
	case 2:
		rt.top = 0;
		rt.left = 0;
		rt.bottom = 480;
		rt.right = 420;

		Xnum = 16; Ynum = 16;
		mine_num = mine = 50;
		break;
	case 3:
		rt.top = 0;
		rt.left = 0;
		rt.bottom = 480;
		rt.right = 700;

		Xnum = 16; Ynum = 30;
		mine_num = mine = 99;
		break;
	}
	SetNewMap(hwnd);
	if (type == gameType::nowPlay) {
		replay.saveLevel(level);
		replay.input(click::startP, -1, -1);
		SetTimer(hwnd, 2, 10, NULL);
		CreateMap();

	}
	else {
		setReplay();
	}
	KillTimer(hwnd, 4);
	tp = chrono::system_clock::now();
	start = chrono::system_clock::to_time_t(tp);
	now = 0;
	mine_num = mine;
	SetTimer(hwnd, 3, 70, NULL);

}