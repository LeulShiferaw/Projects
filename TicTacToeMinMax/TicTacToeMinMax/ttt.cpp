#include "ttt.h"
#include <vector>
#include <iostream>

using namespace std;

//Init board with empty values and initial turn is X
TTT::TTT()
{
	curr_turn = TURN::X;
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
			board[i][j] = BOARD::EMPTY;
	}
}

//Start tic tac toe with initial turn = const Turn &turn
TTT::TTT(const TURN& turn)
{
	curr_turn = turn;
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
			board[i][j] = BOARD::EMPTY;
	}
}

//Return 1 if X won
//Return -1 if O won
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

//switch the turn from O -> X and X -> O
void TTT::switch_turn()
{
	if (curr_turn == TURN::X)
		curr_turn = TURN::O;
	else
		curr_turn = TURN::X;
}

//Make a move to position (x, y)
//Return 1 for successfully made move
//Return -1 for unsuccessful
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

//Generates a move using the minmax algorithm
pair<int, int> TTT::generate_ai_move()
{
	if (curr_turn == TURN::X)
	{
		//maximise
		return max().first;
	}
	else
	{
		//minimise
		return min().first;
	}
}

//Assuming there is still some free space on the board
std::pair<std::pair<int, int>, int> TTT::min()
{
	vector<std::pair<std::pair<int, int>, int>> results;
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			if (board[i][j] == BOARD::EMPTY)
			{
				if (curr_turn == TURN::X)
					board[i][j] = BOARD::X;
				else board[i][j] = BOARD::O;
				switch_turn();

				int curr_status = status();
				if (curr_status == 1)
					results.push_back(make_pair(make_pair(i, j), 1));
				else if (curr_status == -1)
					results.push_back(make_pair(make_pair(i, j), -1));
				else if (curr_status == 0)
					results.push_back(make_pair(make_pair(i, j), 0));
				else
				{
					auto nxt = max();	
					results.push_back(make_pair(make_pair(i, j), nxt.second));
				}

				//Reset
				switch_turn();
				board[i][j] = BOARD::EMPTY();
			}
		}
	}

	if (results.size() == 0)
		throw("Unablle to make move in max function");

	int least_score = INT_MAX;
	std::pair<int, int> move;
	for (const auto &x : results)
	{
		if (x.second < least_score)
		{
			move = x.first;
			least_score = x.second;
		}
	}
	return make_pair(move, least_score);
}

//Assuming there is still some free space on the board
std::pair<std::pair<int, int>, int> TTT::max()
{
	vector<std::pair<std::pair<int, int>, int>> results;
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			if (board[i][j] == BOARD::EMPTY)
			{
				if (curr_turn == TURN::X)
					board[i][j] = BOARD::X;
				else board[i][j] = BOARD::O;
				switch_turn();

				int curr_status = status();
				if (curr_status == 1)
					results.push_back(make_pair(make_pair(i, j), 1));
				else if (curr_status == -1)
					results.push_back(make_pair(make_pair(i, j), -1));
				else if (curr_status == 0)
					results.push_back(make_pair(make_pair(i, j), 0));
				else
				{
					auto nxt = min();	
					results.push_back(make_pair(make_pair(i, j), nxt.second));
				}

				//Reset
				switch_turn();
				board[i][j] = BOARD::EMPTY();
			}
		}
	}

	if (results.size() == 0)
		throw("Unablle to make move in max function");

	int greatest_score = -INT_MAX;
	std::pair<int, int> move;
	for (const auto &x : results)
	{
		if (x.second > greatest_score)
		{
			move = x.first;
			greatest_score = x.second;
		}
	}
	return make_pair(move, greatest_score);
}

//Display the board
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

//To play the game of tic tac toe
void TTT::play_game()
{
	while (true)
	{
		display_board();

		//Player 1 turn
		int x, y;

		//to display the current turn
		string temp = "";
		if (curr_turn == TURN::X)
			temp += "X";
		else
			temp += "O";

		do
		{
			cout << "Enter coordinates(turn = " + temp + "): ";
			cin >> x >> y;
			if (!cin)
				cout << "Invalid Input!";
		} while (!cin);

		//if successfully made move
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

//To play the game of tic tac toe
void TTT::play_ai_game()
{
	while (true)
	{
		display_board();

		//Player 1 turn
		int x, y;

		//to display the current turn
		string temp = "";
		if (curr_turn == TURN::X)
			temp += "X";
		else
			temp += "O";

		do
		{
			cout << "Enter coordinates(turn = " + temp + "): ";
			cin >> x >> y;
			if (!cin)
				cout << "Invalid Input!";
		} while (!cin);

		//if successfully made move
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

		//AI move
		auto ai_move = generate_ai_move();
		if (make_move(ai_move.first, ai_move.second) == 1)
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
