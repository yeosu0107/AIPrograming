#include <windows.h>
#include <time.h>
#include "resource.h"
#include "Replay.h"


LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
LPSTR lpszClass="first";

RECT rt = { 0, 0, 280, 340 };
int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance
					 ,LPSTR lpszCmdParam,int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst=hInstance;
	HACCEL hAcc;
	AdjustWindowRect( &rt, WS_OVERLAPPEDWINDOW, false );

	WndClass.cbClsExtra=0;
	WndClass.cbWndExtra=0;
	WndClass.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor=LoadCursor(NULL,IDC_ARROW);
	WndClass.hIcon=LoadIcon(NULL,IDI_APPLICATION);
	WndClass.hInstance=hInstance;
	WndClass.lpfnWndProc=(WNDPROC)WndProc;
	WndClass.lpszClassName=lpszClass;
	WndClass.lpszMenuName=MAKEINTRESOURCE(IDR_MENU1);
	WndClass.style=CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd=CreateWindow(lpszClass,lpszClass,WS_BORDER | WS_SYSMENU,
		CW_USEDEFAULT,CW_USEDEFAULT,rt.right-rt.left, rt.bottom-rt.top,
		NULL,(HMENU)NULL,hInstance,NULL);
	ShowWindow(hWnd,nCmdShow);
	
	while(GetMessage(&Message,0,0,0)) {
		 
		TranslateMessage(&Message);
		DispatchMessage(&Message);
		
	}
	return Message.wParam;
}
#define up 1
#define down 0
struct grid
{
	int statue;
	int num;
	BOOL mine;
	BOOL flag;
	BOOL balank;
	BOOL error;
	BOOL temp;
	BOOL boom;
	BOOL mouse;
};
int x[8]={-1, 0, 1, -1, 1, -1, 0, 1};
int y[8]={-1, -1, -1, 0, 0, 1, 1, 1};
int Xnum, Ynum, mine;
grid map[30][16];
void CreateMap()
{
	int mine_num=mine;
	srand(time(NULL));
	while(1)
	{
		for(int i=0; i<Ynum; i++) {
			for(int j=0; j<Xnum;j++)
			{
				if(map[i][j].mine==FALSE && rand()%6==2) {
					map[i][j].mine=TRUE;
					mine_num--;
					if(mine_num==0)
						break;
				}
				if(mine_num==0)
					break;
			}
			if(mine_num==0)
				break;
		}
		if(mine_num==0)
			break;
	}
	for(int i=0; i<Ynum; i++)
	{
		for(int j=0; j<Xnum; j++)
		{
			for(int q=0; q<8; q++)
			{
				if(i+y[q]>=0 && i+y[q]<=Ynum && j+x[q]>=0 && j+x[q]<=Xnum)
					if(map[i][j].mine==FALSE && map[i+y[q]][j+x[q]].mine==TRUE)
						map[i][j].num++;
				
			}
			if(map[i][j].num==0 && map[i][j].mine==FALSE)
					map[i][j].balank=TRUE;
		}
	}
}
void CheckBlank( int i, int j)
{
	
	for(int p=-1; p<=1; p++)
	{
		for(int q=-1; q<=1; q++)
		{
			if(i+p<0 || i+p>=Ynum || j+q<0 || j+q>=Xnum)
				continue;
			if(map[i+p][j+q].num!=0 && map[i+p][j+q].statue==up) {
				map[i+p][j+q].statue=down;
				continue;
			}
			else if(map[i+p][j+q].statue==up && map[i+p][j+q].balank==TRUE) {
				map[i+p][j+q].statue=down;
				CheckBlank(i+p, j+q);
			}
			
		}
	}
}
void gameover(HWND hwnd, int select)
{
	KillTimer(hwnd, 2);
	KillTimer(hwnd, 3);
	for(int i=0; i<Ynum; i++) {
		for(int j=0; j<Xnum; j++) {
			if(map[i][j].mine==FALSE && map[i][j].flag==TRUE)
				map[i][j].error=TRUE;
			map[i][j].statue=down;
		}
	}
	switch(select)
	{
	case 1:
		MessageBox(hwnd, "Áö·Ú Æã!!", "Áö·Ú", MB_OK);
		break;
	case 2:
		MessageBox(hwnd, "°Å±â Áö·Ú ¾Æ´Ô", "Áö·Ú", MB_OK);
		break;
	}
	
}
void result(HWND hwnd)
{
	int select=0;
	KillTimer(hwnd, 2);
	KillTimer(hwnd, 3);
	for(int i=0; i<Ynum; i++) {
		for(int j=0; j<Xnum; j++) {
			if(map[i][j].flag==TRUE && map[i][j].mine==FALSE) {
				select=1;
				break;
			}
			if(map[i][j].boom==TRUE) {
				select=2;
				break;
			}
		}
		if(select==1 || select==2)
			break;
	}
	switch(select)
	{
	case 1:
		MessageBox(hwnd, "°Å±â Áö·Ú ¾Æ´Ô", "Áö·Ú", MB_OK);
		break;
	case 0:
		MessageBox(hwnd, "½Â¸®!", "½Â¸®", MB_OK);
		break;
	case 2:
		MessageBox(hwnd, "Áö·Ú Æã!!", "Áö·Ú", MB_OK);
		break;
	}
	
}
void SetNewMap(HWND hwnd)
{
	for(int i=0; i<30; i++)
			for(int j=0; j<16; j++) {
				map[i][j].statue=up;
				map[i][j].mine=FALSE;
				map[i][j].num=0;
				map[i][j].flag=FALSE;
				map[i][j].balank=FALSE;
				map[i][j].error=FALSE;
				map[i][j].temp=FALSE;
				map[i][j].boom=FALSE;
			}
	
	AdjustWindowRect( &rt, WS_OVERLAPPEDWINDOW, false );
	SetWindowPos(hwnd,  HWND_TOP ,100,100,rt.right-rt.left, rt.bottom-rt.top,NULL);
}

void SetLevel(int level, HWND& hwnd, int& time, int& mine_num) {
	switch (level)
	{
	case 1:
		rt.top = 0;
		rt.left = 0;
		rt.bottom = 340;
		rt.right = 280;
		break;
	case 2:
		rt.top = 0;
		rt.left = 0;
		rt.bottom = 480;
		rt.right = 420;
		break;
	case 3:
		rt.top = 0;
		rt.left = 0;
		rt.bottom = 480;
		rt.right = 700;
		break;
	}
	SetNewMap(hwnd);
	CreateMap();
	KillTimer(hwnd, 2);
	time = 0;
	mine_num = mine;
	SetTimer(hwnd, 2, 1000, NULL);
	SetTimer(hwnd, 3, 70, NULL);
}

LRESULT CALLBACK WndProc(HWND hwnd,UINT iMsg,WPARAM wParam,LPARAM lParam)
{
	static HDC hdc, mem1dc, mem2dc, mem3dc, mem4dc;
	static HBITMAP hBit1, BackGround, oldBit1, oldBit2, oldBit3, oldBit4;
	static RECT rectView;
	static HBITMAP tile, Btile, one, two, three, four, five, six, seven, eight, MINE, flag, failflag, boom, timer, nmine, smile, smile2, mouse;
	int t_num=0, f_num=0;
	static int level=1, temp=0, mine_num=0, mx, my,time=0, check=0;
	static BOOL statue, select;
	static char buf[4], buf2[2];
	PAINTSTRUCT ps; 

	Replay replay;

	switch(iMsg)
	{
	case WM_CREATE:
	{
		GetClientRect(hwnd, &rectView);
		SetTimer(hwnd, 1, 70, NULL);
		SetTimer(hwnd, 2, 1000, NULL);
		SetTimer(hwnd, 3, 70, NULL);
		select = FALSE;
		BackGround = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP16));
		tile = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP11));
		Btile = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP2));
		one = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP3));
		two = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP4));
		three = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP5));
		four = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP6));
		five = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP7));
		six = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP8));
		seven = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP9));
		eight = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP10));
		boom = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP14));
		MINE = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP1));
		flag = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP12));
		failflag = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP15));
		timer = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP18));
		nmine = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP17));
		smile = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP19));
		smile2 = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP20));
		mouse = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP21));
		SetNewMap(hwnd);
		Xnum = 9;
		Ynum = 9;
		mine = 10;
		mine_num = mine;
		CreateMap();
	}
		break;
	case WM_MOUSEMOVE:
		mx=HIWORD (lParam);
		my=LOWORD (lParam);
		for(int i=0; i<Ynum;i++) {
			for(int j=0; j<Xnum; j++)
			{
				map[i][j].mouse=FALSE;
			}
		}
		for(int i=0; i<Ynum;i++) {
			for(int j=0; j<Xnum; j++)
			{
				if(map[i][j].statue==up && mx>=j*20+80 && mx<=(j+1)*20+80 && my>=i*20+50 && my<=(i+1)*20+50)
				{
					map[i][j].mouse=TRUE;
				}
			}
		}
		break;
	case WM_LBUTTONDOWN:
		mx=HIWORD (lParam);
		my=LOWORD (lParam);
		statue=TRUE;
		for(int i=0; i<Ynum;i++) {
			for(int j=0; j<Xnum; j++)
			{
				//ºó Ä­ ´©¸£±â
				if(map[i][j].statue==up && mx>=j*20+80 && mx<=(j+1)*20+80 && my>=i*20+50 && my<=(i+1)*20+50)
				{
					replay.input(click::left, j, i);
					map[i][j].statue=down;
					if(map[i][j].mine==TRUE) {
						map[i][j].boom=TRUE;
						gameover(hwnd, 1);
					}
					if(map[i][j].balank==TRUE)
						CheckBlank( i, j);
				}
			}
		}
		//ÀÌ¸ðÆ¼ÄÜ ´©¸£±â
		if(my>rt.right/2-20 && mx>20 && my< rt.right/2+10 && mx<50) {
			select=TRUE;
			SetLevel(level, hwnd, time, mine_num);
		}
		
		break;
	case WM_LBUTTONUP:
		statue=FALSE;
		select=FALSE;
		break;
	case WM_RBUTTONDOWN:
		mx=HIWORD (lParam);
		my=LOWORD (lParam);
		for(int i=0; i<Ynum;i++) {
			for(int j=0; j<Xnum; j++)
			{
				if(statue==TRUE && map[i][j].statue==down && mx>=j*20+80 && mx<=(j+1)*20+80 && my>=i*20+50 && my<=(i+1)*20+50)
				{
					
					t_num=map[i][j].num;
					for(int q=0; q<8; q++)
					{
						if(map[i+y[q]][j+x[q]].flag==TRUE)
							f_num++;
						if(map[i+y[q]][j+x[q]].mine==FALSE && map[i+y[q]][j+x[q]].flag==TRUE)
							gameover(hwnd, 2);
						
						else {
							if(map[i+y[q]][j+x[q]].statue==up)
								map[i+y[q]][j+x[q]].temp=TRUE;
						}
					}
					if(f_num==t_num)
					{
						CheckBlank( i, j);
						for(int q=0; q<8; q++)
						{
							if(map[i+y[q]][j+x[q]].temp==TRUE)
								map[i+y[q]][j+x[q]].statue=down;
						}
					}
				}
				if(map[i][j].statue==up && mx>=j*20+80 && mx<=(j+1)*20+80 && my>=i*20+50 && my<=(i+1)*20+50)
				{
					map[i][j].statue=down;
					map[i][j].flag=TRUE;
					mine_num--;
				}
				else if(map[i][j].statue==down && map[i][j].flag==TRUE && mx>=j*20+80 && mx<=(j+1)*20+80 && my>=i*20+50 && my<=(i+1)*20+50) {
					map[i][j].statue=up;
					map[i][j].flag=FALSE;
					mine_num++;
				
				}	
			}
			t_num=0;
			f_num=0;
		}
		break;
	case WM_RBUTTONUP:
		for(int i=0; i<Ynum;i++) {
			for(int j=0; j<Xnum; j++) {
				map[i][j].temp=FALSE;
			}
		}
		break;
	case WM_PAINT:
	{
		hdc = BeginPaint(hwnd, &ps);
		GetClientRect(hwnd, &rectView);
		mem1dc = CreateCompatibleDC(hdc);
		oldBit1 = (HBITMAP)SelectObject(mem1dc, hBit1);
		BitBlt(hdc, 0, 0, rectView.right, rectView.bottom, mem1dc, 0, 0, SRCCOPY);
		itoa(time, buf, 10);
		itoa(mine_num, buf2, 10);
		SetBkMode(hdc, TRANSPARENT);
		TextOut(hdc, rt.left + 100, rt.bottom - 63, buf, strlen(buf));
		TextOut(hdc, rt.right - 115, rt.bottom - 63, buf2, strlen(buf2));
		SelectObject(mem1dc, oldBit1);
		DeleteDC(mem2dc);
		EndPaint(hwnd, &ps);
	}
		return 0;
	case WM_TIMER:
		GetClientRect(hwnd, &rectView);
		switch(wParam)
		{
		case 1:
			temp=0;
			hdc = GetDC(hwnd);
			if (hBit1 == NULL)
				hBit1 = CreateCompatibleBitmap (hdc, 1024, 768);
			
			mem1dc = CreateCompatibleDC (hdc);
			mem2dc = CreateCompatibleDC (mem1dc);
			mem3dc = CreateCompatibleDC (mem1dc);
			mem4dc = CreateCompatibleDC (mem1dc);
			oldBit1 = (HBITMAP) SelectObject (mem1dc, hBit1); 
			oldBit2 = (HBITMAP) SelectObject (mem2dc, tile); 
			oldBit3 = (HBITMAP) SelectObject (mem3dc, Btile); 
			oldBit4 = (HBITMAP) SelectObject (mem4dc, BackGround); 
			StretchBlt(mem1dc, 0, 0, rectView.right, rectView.bottom, mem4dc, 0, 0,200,400, SRCCOPY);
			for(int i=0; i<Ynum; i++) {
				for(int j=0; j<Xnum; j++) {
					if(map[i][j].statue==up) {
						BitBlt(mem1dc, i*20+50, j*20+80, 20, 20, mem2dc, 0, 0, SRCCOPY);
						if(map[i][j].temp==TRUE) {
							oldBit3 = (HBITMAP) SelectObject (mem3dc, Btile); 
							BitBlt(mem1dc, i*20+50, j*20+80, 20, 20, mem3dc, 0, 0, SRCCOPY);
						}
						if(map[i][j].mouse==TRUE) {
							oldBit3 = (HBITMAP) SelectObject (mem3dc, mouse); 
							BitBlt(mem1dc, i*20+50, j*20+80, 20, 20, mem3dc, 0, 0, SRCCOPY);
						}
					}
					else {
						oldBit3 = (HBITMAP) SelectObject (mem3dc, Btile); 
						BitBlt(mem1dc, i*20+50, j*20+80, 20, 20, mem3dc, 0, 0, SRCCOPY);
						if(map[i][j].mine==TRUE) {
							oldBit3 = (HBITMAP) SelectObject (mem3dc, MINE); 
							BitBlt(mem1dc, i*20+50, j*20+80, 20, 20, mem3dc, 0, 0, SRCCOPY);
						}
						if(map[i][j].boom==TRUE) {
							oldBit3 = (HBITMAP) SelectObject (mem3dc, boom); 
							BitBlt(mem1dc, i*20+50, j*20+80, 20, 20, mem3dc, 0, 0, SRCCOPY);
						}
						if(map[i][j].num==1) {
							oldBit3 = (HBITMAP) SelectObject (mem3dc, one); 
							BitBlt(mem1dc, i*20+50, j*20+80, 20, 20, mem3dc, 0, 0, SRCCOPY);
						}
						if(map[i][j].num==2) {
							oldBit3 = (HBITMAP) SelectObject (mem3dc, two); 
							BitBlt(mem1dc, i*20+50, j*20+80, 20, 20, mem3dc, 0, 0, SRCCOPY);
						}
						if(map[i][j].num==3) {
							oldBit3 = (HBITMAP) SelectObject (mem3dc, three); 
							BitBlt(mem1dc, i*20+50, j*20+80, 20, 20, mem3dc, 0, 0, SRCCOPY);
						}
						if(map[i][j].num==4) {
							oldBit3 = (HBITMAP) SelectObject (mem3dc, four); 
							BitBlt(mem1dc, i*20+50, j*20+80, 20, 20, mem3dc, 0, 0, SRCCOPY);
						}
						if(map[i][j].num==5) {
							oldBit3 = (HBITMAP) SelectObject (mem3dc, five); 
							BitBlt(mem1dc, i*20+50, j*20+80, 20, 20, mem3dc, 0, 0, SRCCOPY);
						}
						if(map[i][j].num==6) {
							oldBit3 = (HBITMAP) SelectObject (mem3dc, six); 
							BitBlt(mem1dc, i*20+50, j*20+80, 20, 20, mem3dc, 0, 0, SRCCOPY);
						}
						if(map[i][j].num==7) {
							oldBit3 = (HBITMAP) SelectObject (mem3dc, seven); 
							BitBlt(mem1dc, i*20+50, j*20+80, 20, 20, mem3dc, 0, 0, SRCCOPY);
						}
						if(map[i][j].num==8) {
							oldBit3 = (HBITMAP) SelectObject (mem3dc, eight); 
							BitBlt(mem1dc, i*20+50, j*20+80, 20, 20, mem3dc, 0, 0, SRCCOPY);
						}
						if(map[i][j].flag==TRUE) {
							oldBit3 = (HBITMAP) SelectObject (mem3dc, flag); 
							BitBlt(mem1dc, i*20+50, j*20+80, 20, 20, mem3dc, 0, 0, SRCCOPY);
						}
						if(map[i][j].error==TRUE) {
							oldBit3 = (HBITMAP) SelectObject (mem3dc, failflag); 
							BitBlt(mem1dc, i*20+50, j*20+80, 20, 20, mem3dc, 0, 0, SRCCOPY);
						}
						
					}
				}
			}
			oldBit4 = (HBITMAP) SelectObject (mem4dc, timer);
			TransparentBlt(mem1dc, rt.left+60, rt.bottom-70, 30, 30, mem4dc, 0,0,80,80, RGB(107,174,255));
			oldBit4 = (HBITMAP) SelectObject (mem4dc, nmine);
			TransparentBlt(mem1dc, rt.right-90, rt.bottom-70, 30, 30, mem4dc, 0,0,80,80, RGB(107,174,255));
			if(select==FALSE) {
				oldBit4 = (HBITMAP) SelectObject (mem4dc, smile);
				TransparentBlt(mem1dc, rt.right/2-20,20, 30, 30, mem4dc, 0,0,595,595, RGB(255,255,255));
			}
			else {
				oldBit4 = (HBITMAP) SelectObject (mem4dc, smile2);
				TransparentBlt(mem1dc, rt.right/2-20,20, 30, 30, mem4dc, 0,0,595,595, RGB(255,255,255));
			}

			SelectObject(mem4dc, oldBit4); DeleteDC(mem4dc);
			SelectObject(mem3dc, oldBit3); DeleteDC(mem3dc);
			SelectObject(mem2dc, oldBit2); DeleteDC(mem2dc);
			SelectObject(mem1dc, oldBit1); DeleteDC(mem1dc);
			ReleaseDC(hwnd, hdc);
			
			break;
		case 2:
			time++;
			if(time==999)
				time+=0;
			break;
		case 3:
			for(int i=0; i<Ynum;i++) {
				for(int j=0; j<Xnum; j++)
				{
					if(map[i][j].statue==up) {
						temp=1;
						break;
					}
					if(i==Ynum-1 && j==Xnum-1) {
						result(hwnd);
					}
				}
				if(temp==1)
					break;
			}
			break;
		}
		InvalidateRgn(hwnd, NULL, FALSE);
		return 0;
	case WM_COMMAND:
		switch(wParam)
		{
		case ID_NEW:
			SetLevel(level, hwnd, time, mine_num);
			break;
		case ID_EXIT:
			PostQuitMessage(0);
			break;
		case ID_EASY:
			level=1;
			Xnum=9; Ynum=9;
			mine_num=mine=10;
			SetLevel(level, hwnd, time, mine_num);
			break;
		case ID_MEDIUM:
			Xnum=16; Ynum=16;
			mine_num=mine=50;
			level=2;
			SetLevel(level, hwnd, time, mine_num);
			break;
		case ID_HARD:
			level=3;
			Xnum=16; Ynum=30;
			mine_num=mine=99;
			SetLevel(level, hwnd, time, mine_num);
			break;
		}
		InvalidateRgn(hwnd, NULL, FALSE);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return(DefWindowProc(hwnd,iMsg,wParam,lParam));
}