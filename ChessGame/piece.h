#pragma once

#include <iostream>

enum class PieceColor {WHITE, BLACK};
enum class PieceType {PAWN, ROOK, KNIGHT, BISHOP, QUEEN, KING};

class Piece{
    private:
        PieceColor pc;
        PieceType  pt;
    public:
        //Initial functions
        Piece() :pc(PieceColor::WHITE), pt(PieceType::PAWN){}
        Piece(PieceColor, PieceType);
        Piece(const Piece &);
        Piece(Piece &&);//move constructor
        Piece &operator=(const Piece &);

        //Getters and setters
        PieceColor getColor() const { return pc; }
        PieceType  getPieceType() const { return pt; }
        void setColor(PieceColor col) { pc = col;}
        void setType(PieceType typ) { pt = typ; }

        //Get the character to display for the piece
        char getChar() const;

        friend std::ostream &operator<<(std::ostream &out, const Piece &p)
        {
            out << p.getChar();
            return out;
        }
};
