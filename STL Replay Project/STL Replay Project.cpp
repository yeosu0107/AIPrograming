//#include <windows.h>
#include "stdafx.h"

#include "resource.h"



LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


LPCWSTR lpszClass = L"지뢰찾기";


Replay	replay;
grid	map[30][16];
HWND	slotHwnd;
HINSTANCE g_hInst;
RECT rt = { 0, 0, 280, 340 };


int x[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };
int y[8] = { -1, -1, -1, 0, 0, 1, 1, 1 };
int Xnum, Ynum, mine;

chrono::time_point<chrono::system_clock> tp = chrono::system_clock::now();
time_t start = chrono::system_clock::to_time_t(tp);
time_t now;

INT_PTR CALLBACK SlotProc(HWND, UINT, WPARAM, LPARAM);

struct tData {
	grid map[30][16];
};
vector<tData> Scene;
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance
	, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDC_STLREPLAYPROJECT);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);



	hWnd = CreateWindow(lpszClass, lpszClass, WS_BORDER | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, rt.right - rt.left, rt.bottom - rt.top,
		NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, 0, 0, 0)) {

		TranslateMessage(&Message);
		DispatchMessage(&Message);

	}
	return Message.wParam;
}



LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static HDC hdc, mem1dc, mem2dc, mem3dc, mem4dc;
	static HBITMAP hBit1, BackGround, oldBit1, oldBit2, oldBit3, oldBit4;
	static RECT rectView;
	static HBITMAP tile, Btile, one, two, three, four, five, six, seven, eight, MINE, flag, failflag, boom, nmine, smile, smile2, mouse;
	int t_num = 0, f_num = 0;
	static int level = 1, temp = 0, mine_num = 0, mx, my, check = 0;
	static bool statue, select;
	static TCHAR buf[10], buf2[20];
	PAINTSTRUCT ps;

	static int type;
	static vData inputData;
	static vData::iterator iter;
	
	static time_t timeCount;
	tData gridData;

	static bool replayWork = false;
	static size_t replayIndex = -1;
	
	switch (iMsg)
	{
	case WM_CREATE:
	{
		GetClientRect(hwnd, &rectView);
		SetTimer(hwnd, 1, 70, NULL);
		
		select = false;
		//배경화면
		BackGround = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP1));
		//숫자
		one = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP6));
		two = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP7));
		three = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP8));
		four = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP9));
		five = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP10));
		six = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP11));
		seven = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP12));
		eight = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP13));
		//지뢰
		boom = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP2));
		MINE = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP4));
		failflag = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP3));
		flag = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP18));
		//UI관련
		nmine = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP20));
		smile = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP15));
		smile2 = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP16));
		//타일
		mouse = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP17));
		tile = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP14));
		Btile = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP5));

		type = gameType::nowPlay;
		SetLevel(level, hwnd, mine_num, type);
	}
	break;
	case WM_MOUSEMOVE:
	{
		if (type != gameType::nowPlay)
			break;
		mx = HIWORD(lParam);
		my = LOWORD(lParam);
		for (int i = 0; i < Ynum; i++) {
			for (int j = 0; j < Xnum; j++)
			{
				map[i][j].mouse = false;
			}
		}
		for (int i = 0; i < Ynum; i++) {
			for (int j = 0; j < Xnum; j++)
			{
				if (map[i][j].statue == up && mx >= j * 20 + 80 && mx <= (j + 1) * 20 + 80 && my >= i * 20 + 50 && my <= (i + 1) * 20 + 50)
				{
					map[i][j].mouse = true;
				}
			}
		}
	}
	break;
	case WM_LBUTTONDOWN:
	{
		if (type != gameType::nowPlay)
			break;
		mx = HIWORD(lParam);
		my = LOWORD(lParam);
		statue = true;
		for (int i = 0; i < Ynum; i++) {
			for (int j = 0; j < Xnum; j++)
			{
				//빈 칸 누르기
				if (map[i][j].statue == up && mx >= j * 20 + 80 && mx <= (j + 1) * 20 + 80 && my >= i * 20 + 50 && my <= (i + 1) * 20 + 50)
				{
					ClickBlank(hwnd, j, i, gameType::nowPlay);
				}
			}
		}
		//이모티콘 누르기
		if (my > rt.right / 2 - 20 && mx > 20 && my < rt.right / 2 + 10 && mx < 50) {
			select = true;
			SetLevel(level, hwnd, mine_num, gameType::nowPlay);
		}
	}
	break;
	case WM_LBUTTONUP:
	{
		if (type != gameType::nowPlay)
			break;
		statue = false;
		select = false;
	}
	break;
	case WM_RBUTTONDOWN:
	{
		if (type != gameType::nowPlay)
			break;
		mx = HIWORD(lParam);
		my = LOWORD(lParam);
		for (int i = 0; i < Ynum; i++) {
			for (int j = 0; j < Xnum; j++)
			{
				if (mx >= j * 20 + 80 && mx <= (j + 1) * 20 + 80 && my >= i * 20 + 50 && my <= (i + 1) * 20 + 50) {
					if (statue == true && map[i][j].statue == down)
						ClickLR(hwnd, j, i, t_num, f_num, gameType::nowPlay);
					else
						ClickFlag(j, i, mine_num);
				}
			}
			t_num = 0;
			f_num = 0;
		}
	}
	break;
	case WM_RBUTTONUP:
	{
		if (type != gameType::nowPlay)
			break;
		UpLR();
	}
	break;
	case WM_PAINT:
	{
		hdc = BeginPaint(hwnd, &ps);
		GetClientRect(hwnd, &rectView);
		mem1dc = CreateCompatibleDC(hdc);
		oldBit1 = (HBITMAP)SelectObject(mem1dc, hBit1);
		BitBlt(hdc, 0, 0, rectView.right, rectView.bottom, mem1dc, 0, 0, SRCCOPY);
		wsprintf(buf2, L"time : %d", now);
		wsprintf(buf, L"%d", mine_num);
		SetBkMode(hdc, TRANSPARENT);
		TextOut(hdc, rt.right / 2 + 10, rt.bottom - 63, buf, lstrlen(buf));
		TextOut(hdc, rt.right / 2 + 50, rt.bottom - 63, buf2, lstrlen(buf2));
		SelectObject(mem1dc, oldBit1);
		DeleteDC(mem2dc);
		EndPaint(hwnd, &ps);
	}
	return 0;
	case WM_KEYDOWN:
	{
		switch (wParam) {
		case VK_LEFT:
			if (!replayWork) {
				if (iter == inputData.begin())
					break;
				iter -= 1;
				int tmpp = iter->getTime();
				now = tmpp - start;
				replayIndex = iter - inputData.begin();
				copy(&Scene[replayIndex].map[0][0], &Scene[replayIndex].map[16][30], &map[0][0]);
			}
			break;
		case VK_RIGHT:
			if (!replayWork) {
				if (iter == inputData.end()-1)
					break;
				int tmpp = iter->getTime();
				now = tmpp - start;
				replayIndex = iter - inputData.begin();
				getReplay(iter, hwnd, t_num, f_num, mine_num, gameType::nowReplay);
				if (Scene.size() < replayIndex + 1) {
					copy(&map[0][0], &map[16][30], &gridData.map[0][0]);
					Scene.emplace_back(gridData);
				}
				iter += 1;
				
			}

			break;
		case VK_RETURN:
			if (replayWork) {
				replayWork = false;
				KillTimer(hwnd, 4);
			}
			else {
				replayWork = true; 
				timeCount = now + start;
				SetTimer(hwnd, 4, 1000, NULL);
			}
			break;
		}
		InvalidateRgn(hwnd, NULL, false);
	}
	break;
	case WM_TIMER:
		GetClientRect(hwnd, &rectView);
		switch (wParam)
		{
		case 1: //랜더링
		{
			temp = 0;
			hdc = GetDC(hwnd);
			if (hBit1 == NULL)
				hBit1 = CreateCompatibleBitmap(hdc, 1024, 768);

			mem1dc = CreateCompatibleDC(hdc);
			mem2dc = CreateCompatibleDC(mem1dc);
			mem3dc = CreateCompatibleDC(mem1dc);
			mem4dc = CreateCompatibleDC(mem1dc);
			oldBit1 = (HBITMAP)SelectObject(mem1dc, hBit1);
			oldBit2 = (HBITMAP)SelectObject(mem2dc, tile);
			oldBit3 = (HBITMAP)SelectObject(mem3dc, Btile);
			oldBit4 = (HBITMAP)SelectObject(mem4dc, BackGround);
			StretchBlt(mem1dc, 0, 0, rectView.right, rectView.bottom, mem4dc, 0, 0, 200, 400, SRCCOPY);
			for (int i = 0; i < Ynum; i++) {
				for (int j = 0; j < Xnum; j++) {
					if (map[i][j].statue == up) {
						BitBlt(mem1dc, i * 20 + 50, j * 20 + 80, 20, 20, mem2dc, 0, 0, SRCCOPY);
						if (map[i][j].temp == true) {
							oldBit3 = (HBITMAP)SelectObject(mem3dc, Btile);
							BitBlt(mem1dc, i * 20 + 50, j * 20 + 80, 20, 20, mem3dc, 0, 0, SRCCOPY);
						}
						if (map[i][j].mouse == true) {
							oldBit3 = (HBITMAP)SelectObject(mem3dc, mouse);
							BitBlt(mem1dc, i * 20 + 50, j * 20 + 80, 20, 20, mem3dc, 0, 0, SRCCOPY);
						}
					}
					else {
						oldBit3 = (HBITMAP)SelectObject(mem3dc, Btile);
						BitBlt(mem1dc, i * 20 + 50, j * 20 + 80, 20, 20, mem3dc, 0, 0, SRCCOPY);
						if (map[i][j].mine == true) {
							oldBit3 = (HBITMAP)SelectObject(mem3dc, MINE);
							BitBlt(mem1dc, i * 20 + 50, j * 20 + 80, 20, 20, mem3dc, 0, 0, SRCCOPY);
						}
						if (map[i][j].boom == true) {
							oldBit3 = (HBITMAP)SelectObject(mem3dc, boom);
							BitBlt(mem1dc, i * 20 + 50, j * 20 + 80, 20, 20, mem3dc, 0, 0, SRCCOPY);
						}
						if (map[i][j].num == 1) {
							oldBit3 = (HBITMAP)SelectObject(mem3dc, one);
							BitBlt(mem1dc, i * 20 + 50, j * 20 + 80, 20, 20, mem3dc, 0, 0, SRCCOPY);
						}
						if (map[i][j].num == 2) {
							oldBit3 = (HBITMAP)SelectObject(mem3dc, two);
							BitBlt(mem1dc, i * 20 + 50, j * 20 + 80, 20, 20, mem3dc, 0, 0, SRCCOPY);
						}
						if (map[i][j].num == 3) {
							oldBit3 = (HBITMAP)SelectObject(mem3dc, three);
							BitBlt(mem1dc, i * 20 + 50, j * 20 + 80, 20, 20, mem3dc, 0, 0, SRCCOPY);
						}
						if (map[i][j].num == 4) {
							oldBit3 = (HBITMAP)SelectObject(mem3dc, four);
							BitBlt(mem1dc, i * 20 + 50, j * 20 + 80, 20, 20, mem3dc, 0, 0, SRCCOPY);
						}
						if (map[i][j].num == 5) {
							oldBit3 = (HBITMAP)SelectObject(mem3dc, five);
							BitBlt(mem1dc, i * 20 + 50, j * 20 + 80, 20, 20, mem3dc, 0, 0, SRCCOPY);
						}
						if (map[i][j].num == 6) {
							oldBit3 = (HBITMAP)SelectObject(mem3dc, six);
							BitBlt(mem1dc, i * 20 + 50, j * 20 + 80, 20, 20, mem3dc, 0, 0, SRCCOPY);
						}
						if (map[i][j].num == 7) {
							oldBit3 = (HBITMAP)SelectObject(mem3dc, seven);
							BitBlt(mem1dc, i * 20 + 50, j * 20 + 80, 20, 20, mem3dc, 0, 0, SRCCOPY);
						}
						if (map[i][j].num == 8) {
							oldBit3 = (HBITMAP)SelectObject(mem3dc, eight);
							BitBlt(mem1dc, i * 20 + 50, j * 20 + 80, 20, 20, mem3dc, 0, 0, SRCCOPY);
						}
						if (map[i][j].flag == true) {
							oldBit3 = (HBITMAP)SelectObject(mem3dc, flag);
							BitBlt(mem1dc, i * 20 + 50, j * 20 + 80, 20, 20, mem3dc, 0, 0, SRCCOPY);
						}
						if (map[i][j].error == true) {
							oldBit3 = (HBITMAP)SelectObject(mem3dc, failflag);
							BitBlt(mem1dc, i * 20 + 50, j * 20 + 80, 20, 20, mem3dc, 0, 0, SRCCOPY);
						}

					}
				}
			}
			/*oldBit4 = (HBITMAP)SelectObject(mem4dc, timer);
			TransparentBlt(mem1dc, rt.left + 60, rt.bottom - 70, 30, 30, mem4dc, 0, 0, 80, 80, RGB(107, 174, 255));*/
			oldBit4 = (HBITMAP)SelectObject(mem4dc, nmine);
			TransparentBlt(mem1dc, rt.right / 2 - 30, rt.bottom - 70, 30, 30, mem4dc, 0, 0, 80, 80, RGB(107, 174, 255));
			if (select == false) {
				oldBit4 = (HBITMAP)SelectObject(mem4dc, smile);
				TransparentBlt(mem1dc, rt.right / 2 - 20, 20, 30, 30, mem4dc, 0, 0, 595, 595, RGB(255, 255, 255));
			}
			else {
				oldBit4 = (HBITMAP)SelectObject(mem4dc, smile2);
				TransparentBlt(mem1dc, rt.right / 2 - 20, 20, 30, 30, mem4dc, 0, 0, 595, 595, RGB(255, 255, 255));
			}

			SelectObject(mem4dc, oldBit4); DeleteDC(mem4dc);
			SelectObject(mem3dc, oldBit3); DeleteDC(mem3dc);
			SelectObject(mem2dc, oldBit2); DeleteDC(mem2dc);
			SelectObject(mem1dc, oldBit1); DeleteDC(mem1dc);
			ReleaseDC(hwnd, hdc);
		}
		break;
		case 2: //타이머
			tp = chrono::system_clock::now();
			now = chrono::system_clock::to_time_t(tp)- start;
			break;
		case 3: //랜더링
		{
			for (int i = 0; i < Ynum; i++) {
				for (int j = 0; j < Xnum; j++)
				{
					if (map[i][j].statue == up) {
						temp = 1;
						break;
					}
					if (i == Ynum - 1 && j == Xnum - 1) {
						result(hwnd, type);
					}
				}
				if (temp == 1)
					break;
			}
		}
		break;
		case 4: //리플레이
		{
			int tmpp = iter->getTime();
			now = timeCount - start;
			replayIndex = iter - inputData.begin();

			if (timeCount != tmpp) {
				timeCount++;
				break;
			}
			if (iter >= inputData.end() - 1) {
				KillTimer(hwnd, 4);
				getReplay(iter, hwnd, t_num, f_num, mine_num, gameType::nowReplay);
				copy(&map[0][0], &map[16][30], &gridData.map[0][0]);
				Scene.emplace_back(gridData);
				replayWork = false;
				break;
			}
			getReplay(iter, hwnd, t_num, f_num, mine_num, gameType::nowReplay);
			copy(&map[0][0], &map[16][30], &gridData.map[0][0]);
			Scene.emplace_back(gridData);
			iter += 1;
			if (iter->getTime() != tmpp) 
				timeCount++;
			break;
		}
		break;
		case 5: //리플레이
		{
			UpLR();
			KillTimer(hwnd, 5);
		}
		break;
		}
		InvalidateRgn(hwnd, NULL, false);
		return 0;
	case WM_COMMAND:
		switch (wParam)
		{
		case ID_NEW:
			SetLevel(level, hwnd, mine_num, gameType::nowPlay);
			type = gameType::nowPlay;
			break;
		case ID_EXIT:
			PostQuitMessage(0);
			break;
		case ID_EASY:
			level = 1;
			type = gameType::nowPlay;
			SetLevel(level, hwnd, mine_num, type);
			break;
		case ID_MEDIUM:
			level = 2;
			type = gameType::nowPlay;
			SetLevel(level, hwnd, mine_num, type);
			break;
		case ID_HARD:
			level = 3;
			type = gameType::nowPlay;
			SetLevel(level, hwnd, mine_num, type);
			break;
		case ID_REPLAY_SLOT1:
			if (!replay.fileOpen("replay1.txt")) {
				MessageBox(hwnd, L"리플레이 파일이\n존재하지않습니다.", L"에러", MB_OK);
				break;
			}
			initReplay(inputData, iter, type, timeCount, mine_num, hwnd);
			replayWork = true;
			break;
		case ID_REPLAY_SLOT2:
			if (!replay.fileOpen("replay2.txt")) {
				MessageBox(hwnd, L"리플레이 파일이\n존재하지않습니다.", L"에러", MB_OK);
				break;
			}
			initReplay(inputData, iter, type, timeCount, mine_num, hwnd);		
			replayWork = true;
			break;
		case ID_REPLAY_SLOT3:
			if (!replay.fileOpen("replay3.txt")) {
				MessageBox(hwnd, L"리플레이 파일이\n존재하지않습니다.", L"에러", MB_OK);
				break;
			}
			initReplay(inputData, iter, type, timeCount, mine_num, hwnd);
			replayWork = true;
			break;
		}
		InvalidateRgn(hwnd, NULL, false);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}

INT_PTR CALLBACK SlotProc(HWND hDig, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage) {
	case WM_COMMAND:
	{
		switch (LOWORD(wParam)) {
		case IDC_REPLAY_SLOT1:
			replay.fileSave("replay1.txt");
			DestroyWindow(hDig);
			hDig = NULL;
			return true;
		case IDC_REPLAY_SLOT2:
			replay.fileSave("replay2.txt");
			DestroyWindow(hDig);
			hDig = NULL;
			return true;
		case IDC_REPLAY_SLOT3:
			replay.fileSave("replay3.txt");
			DestroyWindow(hDig);
			hDig = NULL;
			return true;
		default:
			return false;
		}
	}
	case WM_CLOSE:
		DestroyWindow(hDig);
		hDig = NULL;
		return true;
	}
	return 0;
}