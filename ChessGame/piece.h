#pragma once

enum class PieceColor {WHITE, BLACK};
enum class PieceType {PAWN, ROOK, KNIGHT, BISHOP, QUEEN, KING};

class Piece{
    private:
        PieceColor pc;
        PieceType  pt;
    public:
        //Initial functions
        Piece(PieceColor, PieceType);
        Piece(Piece &);
        Piece(Piece &&);//move constructor
        Piece &operator=(const Piece &);

        //Getters and setters
        PieceColor getColor() const { return pc; }
        PieceType  getPieceType() const { return pt; }
        void setColor(const PieceColor &col) { pc = col;}
        void setType(const PieceType &typ) { pt = typ; }
};
