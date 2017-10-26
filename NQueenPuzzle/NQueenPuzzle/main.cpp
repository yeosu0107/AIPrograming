#include "stdafx.h"
#include "HillClimbing.h"
#include "AStar.h"

//A*�� �ذ�
//���������� �ذ�
//������ ����� ���������� �ذ� (�ɼ����� Ⱦ�̵� ON/OFF)

using namespace std;



int main(void)
{
	int num = 3;
	int option = -1;

	cout << "NQueen ���� �����" << endl;
	while (num < 4) {
		cout << "�� ũ�� �Է� (4�̻��� ����) : ";
		cin >> num;

		if (num < 4)
			cout << "�ٽ� �Է��ϼ���" << endl;
		else
			cout <<endl<< num << "Queen ������ �����մϴ�" << endl << endl;
	}
	//�ǻ���
	Board board(num);

	//board.setQueen(0, 2, 0);
	//board.setQueen(1, 0, 1);
	//board.setQueen(2, 3, 2);
	//board.setQueen(3, 1, 3);
	//�ǿ� �� ��ġ
	for (int i = 0; i < num; ++i)
		board.resetQueen(i, i, i);


	cout << "� ������� ã�����?" << endl;
	cout <<endl<< "Astar - 0" << endl << "HillClimbing - 1" << endl
		<< "HillClimbing and autoRestart - 2" << endl << "HillClimbing ant autoRestart and SideWay - 3" << endl << endl;
	
	while (option < 0 || option >3) {
		cout << "�Է�(0~3���� ����) : ";
		cin >> option;

		if (option < 0 || option >3)
			cout << "�ٽ� �Է��Ͻÿ�" << endl;
	}
	cout << endl;
	if (option == OPTION::ASTAR)
		AStar(num, &board);
		
	else {
		HillClimbing hill(&board, num, option);
		hill.Search();
	}

	return 0;
}