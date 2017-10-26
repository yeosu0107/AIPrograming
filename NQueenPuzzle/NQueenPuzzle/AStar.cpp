#include "AStar.h"
#include <math.h>
#include <queue>

AStar::AStar(int size, Board* board) :
	m_size(size), iSource(board)
{
	//전체 자식노드의 수는 사이즈의 제곱이다.
	m_To = new int[m_size];
	m_From = new int[m_size];
	m_Gcost = new int[m_size];
	m_Fcost = new int[m_size];

	memset(m_To, 0,sizeof(int) * m_size);
	memset(m_From, 0, sizeof(int) * m_size);
	memset(m_Gcost, 0, sizeof(int) * m_size);
	memset(m_Fcost, 0, sizeof(int) * m_size);

	m_ShortestPathTree = new Board*[m_size];
	m_SearchFrontier = new Board*[m_size];

	memset(m_ShortestPathTree, 0, sizeof(Board*)* m_size);
	memset(m_SearchFrontier, 0, sizeof(Board*)* m_size);

	m_ShortestPathTree[0] = iSource;
	m_SearchFrontier[0] = iSource;
	Search();
}


AStar::~AStar()
{
}


void AStar::Search()
{
	queue<pair<int,int>> pq;

	pq.push(make_pair(0,0));

	while (!pq.empty()) {
		int NextClosestNode = pq.front().first; //인덱스
		int nowCost = pq.front().second; //비용
		pq.pop();

		m_ShortestPathTree[NextClosestNode] = m_SearchFrontier[NextClosestNode];

		//결과 찾음
		if (m_ShortestPathTree[NextClosestNode]->CollisionCheck() == 0) return;


		vector<Board*> Node;
		//인접노드 찾기
		searchCloseNode(NextClosestNode, Node);

		for (const auto&pE : Node) {
			std::cout << pE->CollisionCheck() << std::endl;
		}
	}
}

void AStar::searchCloseNode(int NextClosestNode, vector<Board*>& save) {
	Board* m_Board = m_ShortestPathTree[NextClosestNode];
	Queen* tmp = m_Board->getQueen();

	for (int i = 0; i < m_size; ++i) {

		int save_x = tmp[i].x;
		int save_y = tmp[i].y;

		for (int x = 0; x < m_size; ++x) {
			int currCost = m_Board->setQueen(i, x, save_y);
			if (currCost == -1)
				continue;
			save.emplace_back(new Board(m_Board));
		}

		for (int y = 0; y < m_size; ++y) {
			int currCost = m_Board->setQueen(i, save_x, y);
			if (currCost == -1)
				continue;
			save.emplace_back(new Board(m_Board));
		}

		int x, y;

		for (x = save_x, y = save_y; x < m_size && y < m_size; ++x, ++y) {
			int currCost = m_Board->setQueen(i, x, y);
			if (currCost == -1)
				continue;
			save.emplace_back(new Board(m_Board));
		}
		for (x = save_x, y = save_y; x >= m_size && y >= m_size; --x, --y) {
			int currCost = m_Board->setQueen(i, x, y);
			if (currCost == -1)
				continue;
			save.emplace_back(new Board(m_Board));
		}
	}
}