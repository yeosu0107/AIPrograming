#include "AStar.h"
#include <math.h>
#include <queue>

AStar::AStar(int size, Board* board) :
	m_size(size), iSource(board)
{
	//전체 자식노드의 수는 사이즈의 제곱이다.
	m_To = new int[(int)pow(m_size,m_size)];
	m_From = new int[(int)pow(m_size, m_size)];
	m_Gcost = new int[(int)pow(m_size, m_size)];
	m_Fcost = new int[(int)pow(m_size, m_size)];

	memset(m_To, -1,sizeof(int) * (int)pow(m_size, m_size));
	memset(m_From, -1, sizeof(int) * (int)pow(m_size, m_size));

	memset(m_Gcost, -1, sizeof(int) * (int)pow(m_size, m_size));
	memset(m_Fcost, -1, sizeof(int) * (int)pow(m_size, m_size));

	m_PathTree = new Board*[(int)pow(m_size, m_size)];
	m_SearchFrontier = new Board*[(int)pow(m_size, m_size)];

	memset(m_PathTree, 0, sizeof(Board*)* (int)pow(m_size, m_size));
	memset(m_SearchFrontier, 0, sizeof(Board*)* (int)pow(m_size, m_size));

	m_PathTree[0] = iSource;
	m_SearchFrontier[0] = iSource;
	
	m_Gcost[0] = board->CollisionCheck();
	m_Fcost[0] = board->CollisionCheck();

	index = 1;
	Search();
}


AStar::~AStar()
{
}


void AStar::Search()
{
	queue<pair<int,int>> pq;

	pq.push(make_pair(0,0));

	int saveCost = -1;
	while (!pq.empty()) {
		int NextClosestNode = pq.front().first; //인덱스
		int nowCost = pq.front().second; //비용
		pq.pop();

		//m_PathTree[NextClosestNode] = m_SearchFrontier[NextClosestNode];
		m_PathTree[NextClosestNode]->printBoard();
		////결과 찾음
		if (m_PathTree[NextClosestNode]->CollisionCheck() == 0) {
			m_PathTree[NextClosestNode]->printBoard();
			return;
		}
			


		vector<pair<Board*, grape>> Node;
		//인접노드 찾기
		searchCloseNode(NextClosestNode, Node);
		
		for (const auto&pE : Node) {
			m_PathTree[pE.second.m_index] = pE.first;
			m_To[pE.second.m_index] = pE.second.m_To;
			m_From[pE.second.m_index] = pE.second.m_From;

			int FCost = m_Fcost[NextClosestNode];
			int GCost = m_Gcost[NextClosestNode] + pE.first->CollisionCheck();
			
			int cost = FCost + GCost;
			//코스트가 작을수록 좋은 것
			m_Fcost[pE.second.m_index] = FCost;
			m_Gcost[pE.second.m_index] = GCost;

			if (saveCost == -1) {
				saveCost = cost;
				pq.push(make_pair(pE.second.m_index, cost));
			}
			else if (saveCost < cost) {
				saveCost = cost;
				pq.push(make_pair(pE.second.m_index, cost));
			}
		}
	}
	

}

void AStar::searchCloseNode(int NextClosestNode, vector<pair<Board*, grape>>& save) {
	Board* m_Board = m_PathTree[NextClosestNode];
	Queen* tmp = m_Board->getQueen();

	for (int i = 0; i < m_size; ++i) {

		int save_x = tmp[i].x;
		int save_y = tmp[i].y;

		for (int x = 0; x < m_size; ++x) {
			int currCost = m_Board->setQueen(i, x, save_y);
			if (currCost == -1)
				continue;
			save.emplace_back(make_pair(new Board(m_Board), grape(NextClosestNode, -1, index++)));
		}

		for (int y = 0; y < m_size; ++y) {
			int currCost = m_Board->setQueen(i, save_x, y);
			if (currCost == -1)
				continue;
			save.emplace_back(make_pair(new Board(m_Board), grape(NextClosestNode, -1, index++)));
		}

		int x, y;

		for (x = save_x, y = save_y; x < m_size && y < m_size; ++x, ++y) {
			int currCost = m_Board->setQueen(i, x, y);
			if (currCost == -1)
				continue;
			save.emplace_back(make_pair(new Board(m_Board), grape(NextClosestNode, -1, index++)));
		}
		for (x = save_x, y = save_y; x >= m_size && y >= m_size; --x, --y) {
			int currCost = m_Board->setQueen(i, x, y);
			if (currCost == -1)
				continue;
			save.emplace_back(make_pair(new Board(m_Board), grape(NextClosestNode, -1, index++)));
		}
	}
}