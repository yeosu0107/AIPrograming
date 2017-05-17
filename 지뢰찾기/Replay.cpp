

#include <ctime>
#include <chrono>
#include <fstream>
#include "Replay.h"




Replay::Replay()
{
	
}


Replay::~Replay()
{
	//fileSave("Replay1.txt");
}

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

void Replay::fileOpen(string name)
{
	ifstream in(name);
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
