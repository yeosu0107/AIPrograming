#include "HillClimbing.h"
#include "stdafx.h"
#include <vector>
#include <random>
#include <time.h>


using namespace std;

default_random_engine dre;

DWORD limitTime = 60;

HillClimbing::HillClimbing(Board* board, int size, int option)
	: m_Board(board), m_size(size), m_option(option)
{
}


HillClimbing::~HillClimbing()
{
	
}

void HillClimbing::Search()
{
	int nowCost=100000; //임의의 비용을 넣어줌(터무니없이 큰 값), 0에 가까울 수록 좋은것이므로
	int prevCost;
	int save_x, save_y;
	vector<pair<int, int>> save;
	int count=0;
	int result = 0;

	vector<pair<int, int>> firstBoard;

	dre.seed(time(NULL)); //시드값은 랜덤으로
	uniform_int_distribution<int> index(0, m_size - 1);

	Queen* tmp = m_Board->getQueen();

	std::cout << "찾는중..."<<std::endl;

	for (int i = 0; i < m_size; ++i) {
		firstBoard.emplace_back(tmp[i].x, tmp[i].y);
	} //재시작을 위한 최초 상태 저장

	DWORD firstTime = timeGetTime() * 0.001;

	while (m_Board->CollisionCheck() != 0) {
		int i = index(dre);

		{
			prevCost = nowCost;
			nowCost = m_Board->CollisionCheck();
			save_x = tmp[i].x;
			save_y = tmp[i].y;

			for (int x = 0; x < m_size; ++x) {
				int currCost = m_Board->setQueen(i, x, save_y);
				if (currCost == -1)
					continue;
				if (currCost <= nowCost) {
					if (currCost < nowCost) {
						save.clear();
					}
					nowCost = currCost;
					save.emplace_back(x, save_y);
				}
			}

			for (int y = 0; y < m_size; ++y) {
				int currCost = m_Board->setQueen(i, save_x, y);
				if (currCost == -1)
					continue;
				if (currCost <= nowCost) {
					if (currCost < nowCost) {
						save.clear();
					}
					nowCost = currCost;
					save.emplace_back(save_x, y);
				}
			}

			int x, y;

			for (x = save_x, y = save_y; x < m_size && y < m_size; ++x, ++y) {
				int currCost = m_Board->setQueen(i, x, y);
				if (currCost == -1)
					continue;
				if (currCost <= nowCost) {
					if (currCost < nowCost) {
						save.clear();
					}
					nowCost = currCost;
					save.emplace_back(x, y);
				}
			}
			for (x = save_x, y = save_y; x >= m_size && y >= m_size; --x, --y) {
				int currCost = m_Board->setQueen(i, x, y);
				if (currCost == -1)
					continue;
				if (currCost <= nowCost) {
					if (currCost < nowCost) {
						save.clear();
					}
					nowCost = currCost;
					save.emplace_back(x, y);
				}
			}
			
			
			uniform_int_distribution<int> ui(0, save.size()-1);
			int index = ui(dre);
			m_Board->setQueen(i, save[index].first, save[index].second);
			save.clear();
			count++;
			
			DWORD currTime = timeGetTime() * 0.001;

			//최대 검색 시간 1분
			if (currTime - firstTime > limitTime) {
				std::cout << "검색을 시작한 지 1분이 경과하였습니다. 결과를 찾지 못했습니다." << std::endl;
				return;
			}

			switch (m_option) {
			case OPTION::Base:
				if (nowCost == prevCost) {
					std::cout << "언덕오르기 결과" << std::endl;
					if (nowCost == 0) {
						std::cout << std::endl << "총 " << result << "번 만에 찾았습니다." << std::endl;
						m_Board->printBoard();
					}
					else {
						std::cout << "찾지 못했습니다." << std::endl;
					}
					return;
				}
				break;
			case OPTION::autoRestart:
				if (nowCost == prevCost&& count > 2) {
					count = 0;
					for (int i = 0; i < m_size; ++i) {
						m_Board->resetQueen(i, firstBoard[i].first, firstBoard[i].second);
					}
					result++;
				}
				break;
			case OPTION::autoandSideRestart:
				if (nowCost == prevCost && count > 20) {
					count = 0;
					for (int i = 0; i < m_size; ++i) {
						m_Board->resetQueen(i, firstBoard[i].first, firstBoard[i].second);
					}
					result++;
				}
				break;
			}

		}
		
		
		
	}
	switch (m_option) {
	case OPTION::autoRestart:
		std::cout << "언덕오르기 무작위 재시작 결과" << std::endl;
		break;
	case OPTION::autoandSideRestart:
		std::cout << "언덕오르기 무작위 재시작 및 횡이동 적용 결과" << std::endl;
		break;
	}
	std::cout << std::endl<<"총 " <<result <<"번 재시작해서 찾았습니다." <<std::endl;
	m_Board->printBoard();
}