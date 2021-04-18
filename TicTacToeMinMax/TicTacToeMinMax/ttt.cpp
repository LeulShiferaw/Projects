#include "ttt.h"
#include <iostream>

using namespace std;

TTT::TTT()
{
	curr_turn = TURN::X;
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
			board[i][j] = BOARD::EMPTY;
	}
}

TTT::TTT(const TURN& turn)
{
	curr_turn = turn;
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
			board[i][j] = BOARD::EMPTY;
	}
}

//Return 1 if x won
//Return -1 if o won
//Return 0 if draw
//Return 2 if incomplete
int TTT::status()
{
	//Horizontal
	for (int i = 0; i < 3; ++i)
	{
		bool won = true;
		for (int j = 1; j < 3; ++j)
		{
			if (board[i][j] != board[i][j - 1] || board[i][0] == BOARD::EMPTY)
				won = false;
		}
		if (won)
		{
			if (board[i][0] == BOARD::X)
				return 1;
			else if (board[i][0] == BOARD::O)
				return -1;
		}
	}

	//Vertical
	for (int i = 0; i < 3; ++i)
	{
		bool won = true;
		for (int j = 1; j < 3; ++j)
		{
			if (board[j][i] != board[j - 1][i] || board[0][i] == BOARD::EMPTY)
				won = false;
		}
		if (won)
		{
			if (board[0][i] == BOARD::X)
				return 1;
			else if (board[0][i] == BOARD::O) 
				return -1;
		}
	}

	//Diagonals
	if (board[0][0] != BOARD::EMPTY && board[0][0] == board[1][1] && board[0][0] == board[2][2])
	{
		if (board[0][0] == BOARD::X)
			return 1;
		else if (board[0][0] == BOARD::O)
			return -1;
	}

	if (board[0][2] != BOARD::EMPTY && board[1][1] == board[0][2] && board[0][2] == board[2][0])
	{
		if (board[0][2] == BOARD::X)
			return 1;
		else if (board[0][0] == BOARD::O)
			return -1;
	}

	//Check if it is incomplete
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
			if (board[i][j] == BOARD::EMPTY)
				return 2;
	}

	return 0;
}

void TTT::switch_turn()
{
	if (curr_turn == TURN::X)
		curr_turn = TURN::O;
	else
		curr_turn = TURN::X;
}

int TTT::make_move(int x, int y)
{
	if (x < 0 || x >= 3 || y < 0 || y >= 3)
		throw exception("Out of range error!");

	if (board[x][y] == BOARD::EMPTY) //If there is empty space
	{
		if (curr_turn == TURN::X)
		{
			board[x][y] = BOARD::X;
		}
		else if (curr_turn == TURN::O)
		{
			board[x][y] = BOARD::O;
		}
		return 1; //Return 1 for successfully made move
	}
	return -1; //Return -1 for unsuccessful
}

void TTT::display_board()
{
	cout << "-------" << endl;
	for (int i = 0; i < 3; ++i)
	{
		cout << "|";
		for (int j = 0; j < 3; ++j)
		{
			if (board[i][j] == BOARD::EMPTY)
				cout << " |";
			else if (board[i][j] == BOARD::X)
				cout << "X|";
			else
				cout << "O|";
		}
		cout << " " << i << endl;
		cout << "-------" << endl;
	}
	cout << " 0 1 2" << endl;
}

void TTT::play_game()
{
	while (true)
	{
		display_board();

		//Player 1 turn
		int x, y;

		string temp = "";
		if (curr_turn == TURN::X)
			temp += "X";
		else
			temp += "O";

		cout << "Enter coordinates(" + temp + "): ";
		cin >> x >> y;
		if (make_move(x, y) == 1)
		{
			int temp0 = status();
			if (temp0 == 1)
			{
				display_board();
				cout << "X WON!" << endl;
				return;
			}
			else if (temp0 == 0)
			{
				display_board();
				cout << "DRAW!" << endl;
				return;
			}
			else if (temp0 == -1)
			{
				display_board();
				cout << "O WON!" << endl;
				return;
			}

			switch_turn();
		}
	}
}
