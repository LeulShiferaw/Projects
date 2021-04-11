#include "ttt.h"

TTT::TTT()
{
	curr_turn = TURN::X;
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
			board[i][j] = ' ';
	}
}

TTT::TTT(const TURN& turn)
{
	curr_turn = turn;
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
			board[i][j] = ' ';
	}
}

int TTT::status()
{
	for (int i = 0; i < 3; ++i)
	{
		bool won = true;
		for (int j = 1; j < 3; ++j)
		{
			if (board[i][j] != board[i][j - 1] || board[i][0] == ' ')
				won = false;
		}
		if (won)
		{
			if (board[i][0] == 'X')
				return 1;
			else if (board[i][0] == 'Y')
				return -1;
		}
	}

	for (int i = 0; i < 3; ++i)
	{
		bool won = true;
		for (int j = 1; j < 3; ++j)
		{
			if (board[j][i] != board[j - 1][i] || board[0][i] == ' ')
				won = false;
		}
		if (won)
		{
			if (board[0][i] == 'X')
				return 1;
			else if (board[0][i] == 'O')
				return -1;
		}
	}

	if (board[0][0] != ' ' && board[0][0] == board[1][1] && board[0][0] == board[2][2])
	{
		if (board[0][0] == 'X')
			return 1;
		else if (board[0][0] == 'O')
			return -1;
	}

	if (board[0][2] != ' ' && board[1][1] == board[0][2] && board[0][2] == board[2][0])
	{
		if (board[0][2] == 'X')
			return 1;
		else if (board[0][0] == 'O')
			return -1;
	}
	return 0;
}

void TTT::make_move(int x, int y)
{
	if (x >= 0 && x <= 3 && y >= 0 && y <= 3)
	{
		if (curr_turn == TURN::X)
		{
			board[x][y] = 'X';
		}
		else if (curr_turn == TURN::O)
		{
			board[x][y] = 'O';
		}
	}
}
