#pragma once


//���� �ʱ�ȭ ����
void CreateMap();
void CheckBlank(int i, int j);
void gameover(HWND hwnd, int select, int type);
void result(HWND hwnd, int type);
void SetNewMap(HWND hwnd);

//���� �÷��� ����
void ClickBlank(HWND& hwnd, const int& xPos, const int& yPos, const int& type);
void ClickLR(HWND& hwnd, const int& xPos, const int& yPos, int& t_num, int& f_num, int type);
void UpLR();
void ClickFlag(const int& xPos, const int& yPos, int& mine_num);
void SetLevel(int level, HWND& hwnd, int& mine_num, int type);

