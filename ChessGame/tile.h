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
        Tile(TileColor, bool, Piece=Piece(PieceColor::WHITE, PieceType::PAWN));
        Tile(Tile &);
        Tile(Tile &&);
        Tile &operator=(const Tile &);
};