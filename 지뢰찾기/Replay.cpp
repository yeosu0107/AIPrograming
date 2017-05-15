

#include <ctime>
#include <chrono>
#include <fstream>
#include "Replay.h"




Replay::Replay()
{
	
}


Replay::~Replay()
{
	ofstream out("replay.txt", ios::trunc);
	
	out << level << endl;

	for (auto mp : mine) {
		out << mp.first << "\t" << mp.second << endl;
	}
	
	for (auto p : v) {
		out << p.getTime() << "\t" << p.getType() << "\t" << p.getX() << "\t" << p.getY();
		out << endl;
	}
}

void Replay::input(int type, int x, int y) {
	auto tp = chrono::system_clock::now();
	time_t tmp = chrono::system_clock::to_time_t(tp);

	Data data(tmp, type, x, y);
	v.push_back(data);
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
