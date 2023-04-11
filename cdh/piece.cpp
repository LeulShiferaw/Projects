#include "piece.h"

Piece::Piece(PieceColor col, PieceType type) :pc(col), pt(type) {}

Piece::Piece(Piece &p)
{
    this->pc = p.pc;
    this->pt = p.pt;
}

Piece::Piece(Piece &&p)
{
    this->pc = p.pc;
    this->pt = p.pt;
}

Piece &Piece::operator=(const Piece &p)
{
    this->pc = p.pc;
    this->pt = p.pt;
}