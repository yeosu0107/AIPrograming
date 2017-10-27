#pragma once
#include "Board.h"
#include <vector>

using namespace std;

struct grape
{
	int m_index;
	int m_To;
	int m_From;

	grape(int from, int to, int index) {
		m_To = to;
		m_From = from;
		m_index = index;
	}
};

class AStar
{
private:
	int* m_Gcost;
	int* m_Fcost;
	int* m_From;
	int* m_To;

	Board** m_PathTree;
	Board** m_SearchFrontier;


	Board* iSource;

	int m_size;
	int index = 0;
public:
	AStar(int size, Board* board);
	~AStar();
	void Search();
	void searchCloseNode(int NextClosestNode, vector<pair<Board*,grape>>& save);
};

