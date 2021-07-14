#include "ttt.h"
#include "testing.h"
#include <iostream>

using namespace std;

void test_max_min()
{
	TTT ttt;
	ttt.board[0][0] = BOARD::X;
	/*
	ttt.board[0][1] = BOARD::O; 
	ttt.board[0][2] = BOARD::X;
	ttt.board[1][0] = BOARD::X;
	ttt.board[1][1] = BOARD::O;
	ttt.board[1][2] = BOARD::X;
	ttt.board[2][0] = BOARD::EMPTY;
	ttt.board[2][1] = BOARD::EMPTY;
	ttt.board[2][2] = BOARD::EMPTY;
	*/

	ttt.switch_turn();
	ttt.display_board();
	
	auto res = ttt.generate_ai_move();
	cout << res.first << " " << res.second << endl;
	
}