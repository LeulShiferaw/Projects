#ifndef BOARD_H_INCLUDED
#define BOARD_H_INCLUDED

#include <iostream>

class Board
{
private:
    char board[3][3];
public:
    Board();
    void display();
    void insert_x(const int& x, const int& y)
    {
        board[x][y] = 'X';
    };
    void insert_o(const int& x, const int& y)
    {
        board[x][y] = 'O';
    };

    //1 for x, -1 for o and 0 for draw
    bool winner();
};

#endif // BOARD_H_INCLUDED
