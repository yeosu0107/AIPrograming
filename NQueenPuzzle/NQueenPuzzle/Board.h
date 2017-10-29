#pragma once
#include "stdafx.h"
struct Queen
{
	int x, y;
	Queen() {
		x = 0;
		y = 0;
	}
	Queen(int _x, int _y) {
		x = _x;
		y = _y;
	}
};

class Board
{
private:
	int m_size;
	int** m_board;
	Queen* m_queen;
	int now_queen;
public:
	Board();
	Board(Board* board) :
		m_size(board->m_size), now_queen(board->now_queen)
	{
		this->m_board = new int*[m_size];
		memcpy(m_board, board->m_board, sizeof(int*)*m_size);
		for (int i = 0; i < m_size; ++i) {
			this->m_board[i] = new int[m_size];
			memcpy(m_board[i], board->m_board[i], sizeof(int)*m_size);
		}


		this->m_queen = new Queen[m_size];
		memcpy(m_queen, board->m_queen, sizeof(Queen)*m_size);
	}
	Board(int size);
	~Board();

	int setQueen(int index, int x, int y);
	void resetQueen(int index, int x, int y);
	
	Queen* getQueen()const { return m_queen; }

	void setNowQueen(int t) {
		now_queen = t;
	}
	int getNowQueen() const { return now_queen; }
	int CollisionCheck(); //Queen들의 총 충돌횟수를 반환, 중복체크 안함
	int CollisionCheck2(); //중복체크함
	void printBoard();
};

