#include "tile.h"

Tile::Tile() :has_pc(false), tc(TileColor::WHITE), my_piece(Piece(PieceColor::WHITE, PieceType::PAWN)){ }

Tile::Tile(TileColor col, bool has_pc, Piece myPiece) :tc(col), my_piece(myPiece)
{
    this->has_pc = has_pc;
}

Tile::Tile(const Tile &t) :has_pc(t.has_pc), tc(t.tc), my_piece(t.my_piece){ }

Tile::Tile(Tile &&t) :has_pc(t.has_pc), tc(t.tc), my_piece(t.my_piece) { }

Tile &Tile::operator=(const Tile &t)
{
    this->has_pc = t.has_pc;
    this->my_piece = t.my_piece;
    this->tc = t.tc;
    return *this;
}