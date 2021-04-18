#ifndef TICTACTOE
#define TICTACTOE

enum class TURN { X, O };
enum class BOARD {EMPTY, X, O};

class TTT
{
private:
	BOARD board[3][3];
	TURN curr_turn;
public:
	TTT();
	TTT(const TURN&);
	int status(); //Checks if it is a draw, x won, or o won
	void switch_turn();
	int make_move(int x, int y);
	void display_board();
	void play_game();
};

#endif