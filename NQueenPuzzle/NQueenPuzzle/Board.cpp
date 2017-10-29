#include "Board.h"
#include "stdafx.h"
Board::Board()
{
	m_size = -1;
	m_board = nullptr;
	m_queen = nullptr;
	now_queen = -1;
}

Board::Board(int size) : m_size(size)
{
	m_board = new int*[m_size];
	for (int i = 0; i < m_size; ++i)
		m_board[i] = new int[m_size];

	m_queen = new Queen[m_size];

	for (int i = 0; i < m_size; ++i)
		memset(m_board[i], 0, sizeof(int) * m_size);
	memset(m_queen, 0, sizeof(int) * m_size);

	now_queen = 0;
}

Board::~Board()
{
	delete m_board;
}

int Board::setQueen(int index, int x, int y)
{
	if (x<0 || x>=m_size
		|| y<0 || y>=m_size)
		return -1;
	if (m_board[y][x] != 0)
		return -1;

	int prev_x = m_queen[index].x;
	int prev_y = m_queen[index].y;

	m_board[prev_y][prev_x] = 0;

	Queen tmp(x, y);
	m_queen[index] = tmp;
	
	m_board[y][x] = 1;

	return CollisionCheck();
}

void Board::resetQueen(int index, int x, int y)
{
	int prev_x = m_queen[index].x;
	int prev_y = m_queen[index].y;

	m_board[prev_y][prev_x] = 0;

	Queen tmp(x, y);
	m_queen[index] = tmp;

	m_board[y][x] = 1;
}



int Board::CollisionCheck()
{
	int SumOfCollision = 0;

	for (int i = 0; i < m_size; ++i) {
		int now_x = m_queen[i].x;
		int now_y = m_queen[i].y;

		for (int k = i + 1; k < m_size; ++k) {
			if (now_x == m_queen[k].x) //같은 행에 있으면 충돌가능
				SumOfCollision++;
			if (now_y == m_queen[k].y) //같은 열에 있으면 충돌가능
				SumOfCollision++;
			if ((now_x - m_queen[k].x)*(now_x - m_queen[k].x) == 
				(now_y - m_queen[k].y)*(now_y - m_queen[k].y)) //뺀 값이 같으면
				SumOfCollision++;
		}
	}

	return SumOfCollision;
}

int Board::CollisionCheck2()
{
	int SumOfCollision = 0;

	for (int i = 0; i < m_size; ++i) {
		int now_x = m_queen[i].x;
		int now_y = m_queen[i].y;

		for (int k = 0; k < m_size; ++k) {
			if (now_x == m_queen[k].x) //같은 행에 있으면 충돌가능
				SumOfCollision++;
			if (now_y == m_queen[k].y) //같은 열에 있으면 충돌가능
				SumOfCollision++;
			if ((now_x - m_queen[k].x)*(now_x - m_queen[k].x) ==
				(now_y - m_queen[k].y)*(now_y - m_queen[k].y)) //뺀 값이 같으면
				SumOfCollision++;
		}
	}

	return SumOfCollision;
}

void Board::printBoard()
{
	for (int i = 0; i < m_size; ++i) {
		for (int j = 0; j < m_size; ++j) {

			switch (m_board[i][j]) {
			case 0:
				std::cout << "□";
				break;
			case 1:
				std::cout << "퀸";
				break;
			}
			std::cout << "  ";
		}
		std::cout << std::endl<<std::endl;
	}
}