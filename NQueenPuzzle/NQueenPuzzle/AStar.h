#pragma once
#include <vector>
#include <queue>
#include "Board.h"
using namespace std;

struct Cost
{
	int m_hCost; //ÄýÀÇ °¹¼ö
	int m_gCost; //Ãæµ¹ È½¼ö
	int m_Cost;   //ÃÑ ÄÚ½ºÆ®

	Cost(int h, int g) {
		m_hCost = h;
		m_gCost = g;
		m_Cost = m_hCost + m_gCost;
	}
};

class AStar
{
private:
	int m_size;

	vector<pair<Cost, Board*>> save;
	//queue<pair<Cost, Board*>> frontier;

public:
	AStar(int size);
	~AStar();
	
	void Search();
};