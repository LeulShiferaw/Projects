#pragma once

#include "piece.h"

enum class TileColor {WHITE, BLACK};

class Tile
{
    private:
        bool has_pc;
        TileColor tc;
        Piece my_piece;
    public:
        Tile();
        Tile(TileColor, bool, Piece);
        Tile(const Tile &);
        Tile(Tile &&);
        Tile &operator=(const Tile &);

        bool getHasPC() const { return has_pc; }
        TileColor getTileColor() const { return tc; }
        Piece& getPiece() { return my_piece; }
        void setHasPC(bool t)  { has_pc = t; }
        void setTC(TileColor col) { tc = col; }
        void setPiece(Piece p) {
            my_piece = p;
        }

        friend std::ostream &operator<<(std::ostream &out, const Tile &t)
        {
            if(t.has_pc) out << t.my_piece;
            else if(t.tc == TileColor::WHITE) out << " ";
            else out << "X";
            return out;
        }
};