
#include <utility>
#include <ctime>
#include <chrono>
#include <fstream>
#include "Replay.h"




Replay::Replay()
{
	
}


Replay::~Replay()
{
	ofstream out("replay.txt", ios::app);
	for (auto p : v) {
		out << p.getTime() << " " << p.getType() << " " << p.getX() << " " << p.getY();
		out << endl;
	}
}

void Replay::input(int type, int x, int y) {
	auto tp = chrono::system_clock::now();
	time_t tmp = chrono::system_clock::to_time_t(tp);

	Data data(tmp, type, x, y);
	v.push_back(data);
}