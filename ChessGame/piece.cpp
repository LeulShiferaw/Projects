#include "piece.h"

Piece::Piece(PieceColor col, PieceType type) :pc(col), pt(type) {}

Piece::Piece(const Piece &p)
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
    return *this;
}

char Piece::getChar() const
{
    if(pc == PieceColor::WHITE)
    {
        if(pt == PieceType::PAWN) return 'P';
        else if(pt == PieceType::KNIGHT) return 'N';
        else if(pt == PieceType::BISHOP) return 'B';
        else if(pt == PieceType::ROOK) return 'R';
        else if(pt == PieceType::KING) return 'K';
        else if(pt == PieceType::QUEEN) return 'Q';
    }
    else if(pc == PieceColor::BLACK)
    {
        if(pt == PieceType::PAWN) return 'p';
        else if(pt == PieceType::KNIGHT) return 'n';
        else if(pt == PieceType::BISHOP) return 'b';
        else if(pt == PieceType::ROOK) return 'r';
        else if(pt == PieceType::KING) return 'k';
        else if(pt == PieceType::QUEEN) return 'q';
    }
    return ' ';
}