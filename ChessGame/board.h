#pragma once

#include <vector>
#include <string>
#include <iostream>
#include "tile.h"

enum class Turn {WHITE, BLACK};

const std::string border = " ---------------";
constexpr int DEFAULT_SIZE = 8;

class Board
{
    private:
        Turn turn;
        std::vector<std::vector<Tile>> board;
    public:
        Board();
        Board(const Board &);
        Board(Board &&);

        Turn getTurn() const { return turn; }

        void initial_config();
        void console_display(std::ostream &) const;
        void switch_turn();
        bool make_move(int, int, int, int);
        bool is_correct_move(int, int, int, int);
        bool is_valid_move(int, int);

        friend std::ostream &operator<<(std::ostream &out, const Board &b)
        {
            b.console_display(std::cout);

            out << "TURN: ";
            if(b.turn == Turn::WHITE) out << "WHITE" << std::endl;
            else out << "BLACK" << std::endl;

            return out;
        }
};