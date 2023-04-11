#include "tile.h"

Tile::Tile(TileColor col, bool has_pc, Piece myPiece) :tc(col), my_piece(myPiece)
{
    this->has_pc = has_pc;
}

Tile::Tile(Tile &t) {
    this->has_pc = t.has_pc;
    this->my_piece = t.my_piece;
    this->tc = t.tc;
}

Tile::Tile(Tile &&t)
{
    this->has_pc = t.has_pc;
    this->my_piece = t.my_piece;
    this->tc = t.tc;
}

Tile &Tile::operator=(const Tile &t)
{
    this->has_pc = t.has_pc;
    this->my_piece = t.my_piece;
    this->tc = t.tc;
}