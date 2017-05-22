#pragma once
#include <vector>
#include <utility>
#include <ctime>
#include <chrono>

using namespace std;

enum click { left = 0, right = 1, both = 2, startP=3 };


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
using vData = vector<Data>;
class Replay
{
private:
	vector<Data> v;
	vector<pair<int, int>> mine;
	int level;
	

public:
	Replay() {}
	~Replay() {}

	void input(int type, int x, int y);
	void saveLevel(int level);
	void saveMine(int xPos, int yPos);

	int getLevel() const { return level; }
	vector<pair<int, int>> getMine() const { return mine; }
	vector<Data> getInput() const { return v; }

	bool fileOpen(string name);
	void fileSave(string name);
};

void setReplay();
void getReplay(vData::iterator& iter, HWND& hwnd, int& t_num, int& f_num, int& mine_num, int type);
void initReplay(vData& inputData, vData::iterator& iter,
	int& type, time_t& timeCount, int& mine_num, HWND& hwnd);