#include <fstream>
#include "stdafx.h"
//#include "Replay.h"


void Replay::input(int type, int x, int y) {
	auto tp = chrono::system_clock::now();
	time_t tmp = chrono::system_clock::to_time_t(tp);

	v.emplace_back(tmp, type, x, y);
}

void Replay::saveLevel(int level)
{
	this->level = level;
	v.clear();
	mine.clear();
}

void Replay::saveMine(int xPos, int yPos)
{
	pair<int, int> pos = make_pair(xPos, yPos);
	mine.push_back(pos);
}

bool Replay::fileOpen(string name)
{
	ifstream in(name);
	if (!in)
		return false;
	v.clear();
	mine.clear();
	in >> level;
	int num, x, y;
	time_t tmp;
	int inputType, inputX, inputY;

	if (level == 1)
		num = 10;
	else if (level == 2)
		num = 50;
	else
		num = 99;

	for (int i = 0; i < num; ++i) {
		in >> x >> y;
		mine.emplace_back(make_pair(x,y));
	}
	while (in) {
		tmp = -1;
		in >> tmp >> inputType >> inputX >> inputY;
		if (tmp == -1)
			break;
		v.emplace_back(tmp, inputType, inputX, inputY);
	}
	in.close();
	return true;
}

void Replay::fileSave(string name)
{
	ofstream out(name, ios::trunc);

	out << level;

	for (auto mp : mine) {
		out << endl;
		out << mp.first << "\t" << mp.second;
	}

	for (auto p : v) {
		out << endl;
		out << p.getTime() << "\t" << p.getType() << "\t" << p.getX() << "\t" << p.getY();
		
	}
	out.close();
}

void setReplay() {

	for (const auto& p : replay.getMine()) {
		map[p.second][p.first].mine = true;
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
void getReplay(vData::iterator& iter, HWND& hwnd, int& t_num, int& f_num, int& mine_num, int type) {
	int xPos = iter->getX();
	int yPos = iter->getY();
	int rtype = iter->getType();
	switch (rtype) {
	case click::left:
		ClickBlank(hwnd, xPos, yPos, type);
		break;
	case click::right:
		ClickFlag(xPos, yPos, mine_num);
		break;
	case click::both:
		ClickLR(hwnd, xPos, yPos, t_num, f_num, type);
		SetTimer(hwnd, 5, 200, NULL);
		break;
	case click::startP:
		break;
	}
}

void initReplay(vData& inputData, vData::iterator& iter,
	int& type, time_t& timeCount, int& mine_num, HWND& hwnd) {
	inputData = replay.getInput();
	timeCount = inputData[0].getTime();
	iter = inputData.begin();
	type = gameType::nowReplay;
	SetLevel(replay.getLevel(), hwnd, mine_num, type);

	start = iter->getTime();
	KillTimer(hwnd, 2);
	SetTimer(hwnd, 4, 1000, NULL);
}