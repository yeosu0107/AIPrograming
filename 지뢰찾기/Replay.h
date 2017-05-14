#pragma once
#include <vector>
using namespace std;

enum click { left = 0, right = 1, both = 2 };


class Data
{
private:
	time_t now;  //시간
	int type;	 //우클릭, 좌클릭, 동시클릭
	int xPos;	 
	int yPos;
public:
	Data(time_t tmp, int tp, int x, int y)
		: now(tmp), type(tp), xPos(x), yPos(y) {};
	~Data() {};

	time_t getTime() const { return now; }
	int getType() const { return type; }
	int getX() const { return xPos; }
	int getY() const { return yPos; }
};

class Replay
{
private:
	vector<Data> v;
public:
	Replay();
	~Replay();

	void input(int type, int x, int y);
};

