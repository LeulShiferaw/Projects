#ifndef TICTACTOE
#define TICTACTOE

#include <utility>

enum class TURN { X, O };
enum class BOARD {EMPTY, X, O};

class TTT
{
public: //public for testing purposes
	BOARD board[3][3];
	TURN curr_turn;
public:
	TTT();
	TTT(const TURN&);
	int status(); //Checks if it is a draw, x won, or o won
	void switch_turn();
	int make_move(int x, int y);
	std::pair<int, int> generate_ai_move();
	std::pair<std::pair<int, int>, int> max();
	std::pair<std::pair<int, int>, int> min();
	void display_board();
	void play_game();
	void play_ai_game();
};

#endif