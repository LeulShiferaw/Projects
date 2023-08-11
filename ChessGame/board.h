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
        bool WK;//white castle king side
        bool WQ;//white castle queen side
        bool bk;//black castle king side
        bool bq;//black castle queen side

        bool black_check;
        bool white_check;
        bool black_king_moved;
        bool white_king_moved;
        std::pair<int, int> enpassant;
        int numHalfMov50; //num of half moves for the 50 move rule
        int numMov;
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
        std::string to_fen();
        std::string chessNot(const std::pair<int, int> &);
        std::vector<std::pair<int, int>> generate_all_white();
        std::vector<std::pair<int, int>> generate_all_black();
        bool is_white_check();
        bool is_black_check();
        bool get_white_check() { return white_check; }
        bool get_black_check() { return black_check; }
        bool get_white_king_castle() { return WK; }
        bool get_white_queen_castle() { return WQ; }
        bool get_black_king_castle() { return bk; }
        bool get_black_queen_castle() { return bq; }
        void check_castle();

        friend std::ostream &operator<<(std::ostream &out, const Board &b)
        {
            b.console_display(std::cout);

            out << "TURN: ";
            if(b.turn == Turn::WHITE) out << "WHITE" << std::endl;
            else out << "BLACK" << std::endl;

            return out;
        }
};