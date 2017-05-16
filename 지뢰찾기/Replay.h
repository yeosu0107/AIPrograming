#pragma once
#include <vector>
#include <utility>
using namespace std;

enum click { left = 0, right = 1, both = 2 };


class Data
{
private:
	time_t now;  //�ð�
	int type;	 //��Ŭ��, ��Ŭ��, ����Ŭ��
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
	vector<pair<int, int>> mine;
	int level;
	

public:
	Replay();
	~Replay();

	void input(int type, int x, int y);
	void saveLevel(int level);
	void saveMine(int xPos, int yPos);

	int getLevel() const { return level; }
	vector<pair<int, int>> getMine() const { return mine; }
	vector<Data> getInput() const { return v; }

	void fileOpen(string name);
	void fileSave(string name);
};

