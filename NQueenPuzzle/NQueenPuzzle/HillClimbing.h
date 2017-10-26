#pragma once

#include "Board.h"
class HillClimbing
{
private:
	Board* m_Board;
	int m_size;

	int m_option;

public:
	HillClimbing(Board* board, int size, int option);
	~HillClimbing();

	void Search();
};

