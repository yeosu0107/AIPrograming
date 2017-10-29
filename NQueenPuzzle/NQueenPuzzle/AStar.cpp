#include "AStar.h"
#include "stdafx.h"

#include <algorithm>

using namespace std;

AStar::AStar(int size) :
	m_size(size) 
{

}

AStar::~AStar()
{

}

void AStar::Search()
{

	for (int i = 0; i < m_size; ++i) {
		Board* tBoard = new Board(m_size);
		int index = tBoard->getNowQueen();

		tBoard->resetQueen(index, i, index);
		Cost cost(1,0);
		tBoard->setNowQueen(index + 1);

		save.push_back(make_pair(cost,tBoard));
	}

	while (1) {
		sort(save.begin(), save.end(), [](const pair<Cost, Board*>& pE1, const pair<Cost, Board*>pE2) {
			return (pE1.first.m_Cost < pE2.first.m_Cost) ;
		});

		pair<Cost, Board*> now = save[0];
		
		save.begin() = save.erase(save.begin());

		if (now.second->CollisionCheck() == 0) {
			now.second->printBoard();
			return;
		}
		if (now.first.m_hCost > m_size - 1)
			continue;

		for (int i = 0; i < m_size; ++i) {
			Board* tBoard = new Board(now.second);
			int index = tBoard->getNowQueen();

			tBoard->resetQueen(index, i, index);
			Cost cost(index+1, tBoard->CollisionCheck());
			tBoard->setNowQueen(index + 1);

			save.push_back(make_pair(cost, tBoard));
		}
	}
}