#include <cassert>
#include "board.h"

Board::Board() :turn(Turn::WHITE), board(DEFAULT_SIZE, std::vector<Tile>(DEFAULT_SIZE, Tile())){ 
    initial_config();
}

Board::Board(const Board &b)
{
    turn = b.turn;
    board = b.board;
}

Board::Board(Board &&b)
{
    turn = b.turn;
    board = b.board;
}

void Board::initial_config()
{
    assert(board.size() == DEFAULT_SIZE);
    assert(board[0].size() == DEFAULT_SIZE);

    for(int i = 0; i<2; ++i)
        for(int j = 0; j<DEFAULT_SIZE; ++j)
            board[i][j].setHasPC(true);
    
    for(int i = 2; i<6; ++i)
        for(int j = 0; j<DEFAULT_SIZE; ++j)
            board[i][j].setHasPC(false);
    
    for(int i = 6; i<DEFAULT_SIZE; ++i)
        for(int j = 0; j<DEFAULT_SIZE; ++j)
            board[i][j].setHasPC(true);
    
    //First row
    board[0][0].setPiece(Piece(PieceColor::BLACK, PieceType::ROOK));
    board[0][1].setPiece(Piece(PieceColor::BLACK, PieceType::KNIGHT));
    board[0][2].setPiece(Piece(PieceColor::BLACK, PieceType::BISHOP));
    board[0][3].setPiece(Piece(PieceColor::BLACK, PieceType::KING));
    board[0][4].setPiece(Piece(PieceColor::BLACK, PieceType::QUEEN));
    board[0][5].setPiece(Piece(PieceColor::BLACK, PieceType::KNIGHT));
    board[0][6].setPiece(Piece(PieceColor::BLACK, PieceType::BISHOP));
    board[0][7].setPiece(Piece(PieceColor::BLACK, PieceType::ROOK));

    //Second row
    for(int i = 0; i<DEFAULT_SIZE; ++i)
        board[1][i].setPiece(Piece(PieceColor::BLACK, PieceType::PAWN));

    //Second last row
    for(int i = 0; i<DEFAULT_SIZE; ++i)
        board[6][i].setPiece(Piece(PieceColor::WHITE, PieceType::PAWN));
    
    //Last Row
    board[7][0].setPiece(Piece(PieceColor::WHITE, PieceType::ROOK));
    board[7][1].setPiece(Piece(PieceColor::WHITE, PieceType::KNIGHT));
    board[7][2].setPiece(Piece(PieceColor::WHITE, PieceType::BISHOP));
    board[7][3].setPiece(Piece(PieceColor::WHITE, PieceType::KING));
    board[7][4].setPiece(Piece(PieceColor::WHITE, PieceType::QUEEN));
    board[7][5].setPiece(Piece(PieceColor::WHITE, PieceType::BISHOP));
    board[7][6].setPiece(Piece(PieceColor::WHITE, PieceType::KNIGHT));
    board[7][7].setPiece(Piece(PieceColor::WHITE, PieceType::ROOK));

    //Set tile color
    TileColor start_color = TileColor::WHITE;
    for(int i = 0; i<board.size(); ++i)
    {
        TileColor curr_color = start_color;
        for(int j = 0; j<board[i].size(); ++j)
        {
            board[i][j].setTC(curr_color);
            if(curr_color == TileColor::WHITE) curr_color = TileColor::BLACK;
            else curr_color = TileColor::WHITE;
        }
        if(start_color == TileColor::WHITE) start_color = TileColor::BLACK;
        else start_color = TileColor::WHITE;
    }
}



void Board::console_display(std::ostream &out) const
{
    for(int i=0; i<board.size(); ++i)
    {
        out << border << std::endl;
        for(int j = 0; j<board.size(); ++j)
        {
            out << "|" <<  board[i][j];
        }
        out << "|" << std::endl;
    }
    out << border << std::endl;
}

void Board::switch_turn()
{
    if(turn == Turn::WHITE) turn = Turn::BLACK;
    else turn = Turn::WHITE;
}

bool Board::make_move(int x1, int y1, int x2, int y2)
{
    if(!board[x1][y1].getHasPC()) return false;
    if(turn == Turn::WHITE && board[x1][y1].getPiece().getColor() != PieceColor::WHITE) return false;
    if(turn == Turn::BLACK && board[x1][y1].getPiece().getColor() != PieceColor::BLACK) return false;

    if(is_correct_move(x1, y1, x2, y2))
    {
        board[x2][y2].setHasPC(true);
        board[x2][y2].setPiece(board[x1][y1].getPiece());
        board[x1][y1].setHasPC(false);
        return true;
    }
    else return false;
}

bool isEqual(int x1, int y1, int x2, int y2)
{
    if(x2 == x1 && y1 == y2) return true;
    return false;
}

//Test KNIGHT MOVES!!
bool Board::is_correct_move(int x1, int y1, int x2, int y2)
{
    if(!is_valid_move(x1, y1) || !is_valid_move(x2, y2)) return false;
    if(board[x1][y1].getHasPC() == false) return false;

    if(board[x1][y1].getPiece().getPieceType() == PieceType::ROOK)
    {
        if(y1 == y2)
        {
            for(int i = x1-1; i>=0; --i)
            {
                if(board[i][y1].getHasPC())
                {
                    if(board[i][y1].getPiece().getColor() != board[x1][y1].getPiece().getColor() && i == x2) return true;
                    return false;
                }
                else if(i == x2) return true;
            }
            for(int i = x1+1; i<board.size(); ++i)
            {
                if(board[i][y1].getHasPC())
                {
                    if(board[i][y1].getPiece().getColor() != board[x1][y1].getPiece().getColor() && i == x2) return true;
                    return false;
                }
                else if(i == x2) return true;
            }
        }

        if(x1 == x2)
        {
            for(int i = y1-1; i>=0; --i)
            {
                if(board[x1][i].getHasPC())
                {
                    if(board[x1][i].getPiece().getColor() != board[x1][y1].getPiece().getColor() && i == y2) return true;
                    return false;
                }
                else if(i == y2) return true;
            }
            for(int i = y1+1; i<board[0].size(); ++i)
            {
                if(board[x1][i].getHasPC())
                {
                    if(board[x1][i].getPiece().getColor() != board[x1][y1].getPiece().getColor() && i == y2) return true;
                    return false;
                }
                else if(i == y2) return true;
            }
        }
        return false;
    }
    else if(board[x1][y1].getPiece().getPieceType() == PieceType::KNIGHT)
    {
        if(board[x2][y2].getHasPC() && board[x2][y2].getPiece().getColor() == board[x1][y1].getPiece().getColor()) return false;
        if(is_valid_move(x1-1, y1-2) && isEqual(x1-1, y1-2, x2, y2)) return true;
        if(is_valid_move(x1-1, y1+2) && isEqual(x1-1, y1+2, x2, y2)) return true;
        if(is_valid_move(x1-2, y1-1) && isEqual(x1-2, y1-1, x2, y2)) return true;
        if(is_valid_move(x1-2, y1+1) && isEqual(x1-2, y1+1, x2, y2)) return true;
        if(is_valid_move(x1+1, y1-2) && isEqual(x1+1, y1-2, x2, y2)) return true;
        if(is_valid_move(x1+1, y1+2) && isEqual(x1+1, y1+1, x2, y2)) return true;
        if(is_valid_move(x1+2, y1-1) && isEqual(x1+2, y1-1, x2, y2)) return true;
        if(is_valid_move(x1+2, y1+1) && isEqual(x1+2, y1+1, x2, y2)) return true;
        return false;
    }
    else if(board[x1][y1].getPiece().getPieceType() == PieceType::BISHOP)
    {
        for(int i = 1; i<board.size(); ++i)
        {
            if(!is_valid_move(x1+i, y1+i)) break;
            if(board[x1+i][y1+i].getHasPC())
            {
                if(board[x1+i][y1+i].getPiece().getColor() != board[x1][y1].getPiece().getColor())
                {
                    if(isEqual(x1+i, y1+i, x2, y2)) return true;
                }
                break;
            }
            if(isEqual(x1+i, y1+i, x2, y2)) return true;
        }
        for(int i = 1; i<board.size(); ++i)
        {
            if(!is_valid_move(x1-i, y1-i)) break;
            if(board[x1-i][y1-i].getHasPC())
            {
                if(board[x1-i][y1-i].getPiece().getColor() != board[x1][y1].getPiece().getColor())
                {
                    if(isEqual(x1-i, y1-i, x2, y2)) return true;
                }
                break;
            }
            if(isEqual(x1-i, y1-i, x2, y2)) return true;
        }
        for(int i = 1; i<board.size(); ++i)
        {
            if(!is_valid_move(x1+i, y1-i)) break;
            if(board[x1+i][y1-i].getHasPC())
            {
                if(board[x1+i][y1-i].getPiece().getColor() != board[x1][y1].getPiece().getColor())
                {
                    if(isEqual(x1+i, y1-i, x2, y2)) return true;
                }
                break;
            }
            if(isEqual(x1+i, y1-i, x2, y2)) return true;
        }
        for(int i = 1; i<board.size(); ++i)
        {
            if(!is_valid_move(x1-i, y1+i)) break;
            if(board[x1-i][y1+i].getHasPC())
            {
                if(board[x1-i][y1+i].getPiece().getColor() != board[x1][y1].getPiece().getColor())
                {
                    if(isEqual(x1-i, y1+i, x2, y2)) return true;
                }
                break;
            }
            if(isEqual(x1-i, y1+i, x2, y2)) return true;
        }
        return false;
    }
    else if(board[x1][y1].getPiece().getPieceType() == PieceType::KING)
    {
        if(x1+1 == x2 && y2 == y1)
        {
            if(board[x2][y2].getHasPC())
            {
                if(board[x2][y2].getPiece().getColor() != board[x1][y1].getPiece().getColor())
                    return true;
                return false;
            } 
            else return true;
        }
        else if(x1-1 == x2 && y2 == y1)
        {
            if(board[x2][y2].getHasPC())
            {
                if(board[x2][y2].getPiece().getColor() != board[x1][y1].getPiece().getColor())
                    return true;
                return false;
            } 
            else return true;
        }
        else if(x1 == x2 && y2 == y1+1)
        {
            if(board[x2][y2].getHasPC())
            {
                if(board[x2][y2].getPiece().getColor() != board[x1][y1].getPiece().getColor())
                    return true;
                return false;
            } 
            else return true;
        }
        else if(x1 == x2 && y2 == y1-1)
        {
            if(board[x2][y2].getHasPC())
            {
                if(board[x2][y2].getPiece().getColor() != board[x1][y1].getPiece().getColor())
                    return true;
                return false;
            } 
            else return true;
        }
        return false;
    }
    else if(board[x1][y1].getPiece().getPieceType() == PieceType::QUEEN)
    {
        if(y1 == y2)
        {
            for(int i = x1-1; i>=0; --i)
            {
                if(board[i][y1].getHasPC())
                {
                    if(board[i][y1].getPiece().getColor() != board[x1][y1].getPiece().getColor() && i == x2) return true;
                    return false;
                }
                else if(i == x2) return true;
            }
            for(int i = x1+1; i<board.size(); ++i)
            {
                if(board[i][y1].getHasPC())
                {
                    if(board[i][y1].getPiece().getColor() != board[x1][y1].getPiece().getColor() && i == x2) return true;
                    return false;
                }
                else if(i == x2) return true;
            }
        }

        if(x1 == x2)
        {
            for(int i = y1-1; i>=0; --i)
            {
                if(board[x1][i].getHasPC())
                {
                    if(board[x1][i].getPiece().getColor() != board[x1][y1].getPiece().getColor() && i == y2) return true;
                    return false;
                }
                else if(i == y2) return true;
            }
            for(int i = y1+1; i<board[0].size(); ++i)
            {
                if(board[x1][i].getHasPC())
                {
                    if(board[x1][i].getPiece().getColor() != board[x1][y1].getPiece().getColor() && i == y2) return true;
                    return false;
                }
                else if(i == y2) return true;
            }
        }
        for(int i = 1; i<board.size(); ++i)
        {
            if(!is_valid_move(x1+i, y1+i)) break;
            if(board[x1+i][y1+i].getHasPC())
            {
                if(board[x1+i][y1+i].getPiece().getColor() != board[x1][y1].getPiece().getColor())
                {
                    if(isEqual(x1+i, y1+i, x2, y2)) return true;
                }
                break;
            }
            if(isEqual(x1+i, y1+i, x2, y2)) return true;
        }
        for(int i = 1; i<board.size(); ++i)
        {
            if(!is_valid_move(x1-i, y1-i)) break;
            if(board[x1-i][y1-i].getHasPC())
            {
                if(board[x1-i][y1-i].getPiece().getColor() != board[x1][y1].getPiece().getColor())
                {
                    if(isEqual(x1-i, y1-i, x2, y2)) return true;
                }
                break;
            }
            if(isEqual(x1-i, y1-i, x2, y2)) return true;
        }
        for(int i = 1; i<board.size(); ++i)
        {
            if(!is_valid_move(x1+i, y1-i)) break;
            if(board[x1+i][y1-i].getHasPC())
            {
                if(board[x1+i][y1-i].getPiece().getColor() != board[x1][y1].getPiece().getColor())
                {
                    if(isEqual(x1+i, y1-i, x2, y2)) return true;
                }
                break;
            }
            if(isEqual(x1+i, y1-i, x2, y2)) return true;
        }
        for(int i = 1; i<board.size(); ++i)
        {
            if(!is_valid_move(x1-i, y1+i)) break;
            if(board[x1-i][y1+i].getHasPC())
            {
                if(board[x1-i][y1+i].getPiece().getColor() != board[x1][y1].getPiece().getColor())
                {
                    if(isEqual(x1-i, y1+i, x2, y2)) return true;
                }
                break;
            }
            if(isEqual(x1-i, y1+i, x2, y2)) return true;
        }
        return false;
    }
    else if(board[x1][y1].getPiece().getPieceType() == PieceType::PAWN)
    {
        if(board[x1][y1].getPiece().getColor() == PieceColor::BLACK)
        {
            if(x2 == x1+1 && y1 == y2){
                if(board[x2][y2].getHasPC() == false) return true;
                return false;
            }
            else if(x1 == 1 && x2 == x1+2 && y1 == y2 && board[x1+1][y1].getHasPC() == false && board[x2][y2].getHasPC() == false) return true;
            else if(x2 == x1+1 && y2 == y1-1 && board[x2][y2].getHasPC() && board[x2][y2].getPiece().getColor() != board[x1][y1].getPiece().getColor()) return true;
            else if(x2 == x1+1 && y2 == y1+1 && board[x2][y2].getHasPC() && board[x2][y2].getPiece().getColor() != board[x1][y1].getPiece().getColor()) return true;
        }
        else {
            if(x2 == x1-1 && y1 == y2){
                if(board[x2][y2].getHasPC() == false) return true;
                return false;
            }
            else if(x1 == 6 && x2 == x1-2 && y1 == y2 && board[x1-1][y1].getHasPC() == false && board[x2][y2].getHasPC() == false) return true;
            else if(x2 == x1-1 && y2 == y1-1 && board[x2][y2].getHasPC() && board[x2][y2].getPiece().getColor() != board[x1][y1].getPiece().getColor()) return true;
            else if(x2 == x1-1 && y2 == y1+1 && board[x2][y2].getHasPC() && board[x2][y2].getPiece().getColor() != board[x1][y1].getPiece().getColor()) return true;
        }
        return false;
    }
    return false;
}

bool Board::is_valid_move(int x, int y)
{
    if(x >= 0 && x <board.size() && y >= 0 && y <board[0].size()) return true;
    return false;
}