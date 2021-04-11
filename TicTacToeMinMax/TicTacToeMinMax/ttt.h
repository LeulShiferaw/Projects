#ifndef TICTACTOE
#define TICTACTOE

enum class TURN { X, Y };

class TTT
{
private:
	char board[3][3];
	TURN curr_turn;
public:
	TTT();
	TTT(const TURN&);
	int status(); //Checks if it is a draw, x won, or o won
	void switch_turn()
	{
		if (curr_turn == TURN::X)
			curr_turn = TURN::Y;
		else
			curr_turn = TURN::X;
	}
	void make_move(int x, int y);
};

#endif