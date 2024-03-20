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
    WK = true;
    WQ = true;
    bk = true;
    bq = true;
    enpassant = {-1, -1};
    numHalfMov50 = 0;
    numMov = 1;
    black_check = false;
    white_check = false;
    black_king_moved = false;
    white_king_moved = false;

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
    board[0][3].setPiece(Piece(PieceColor::BLACK, PieceType::QUEEN));
    board[0][4].setPiece(Piece(PieceColor::BLACK, PieceType::KING));
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
    board[7][3].setPiece(Piece(PieceColor::WHITE, PieceType::QUEEN));
    board[7][4].setPiece(Piece(PieceColor::WHITE, PieceType::KING));
    board[7][5].setPiece(Piece(PieceColor::WHITE, PieceType::BISHOP));
    board[7][6].setPiece(Piece(PieceColor::WHITE, PieceType::KNIGHT));
    board[7][7].setPiece(Piece(PieceColor::WHITE, PieceType::ROOK));

    //Set tile color
    TileColor start_color = TileColor::WHITE;
    for(int i = 0; i<(int)board.size(); ++i)
    {
        TileColor curr_color = start_color;
        for(int j = 0; j<(int)board[i].size(); ++j)
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
    for(int i=0; i<(int)board.size(); ++i)
    {
        out << border << std::endl;
        for(int j = 0; j<(int)board.size(); ++j)
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

    check_castle();//Assigns WK, WQ, bk, bq for white castle king, queen then black castle king, queen

    auto curr_enpassant = enpassant;
    auto start_tile = board[x1][y1];
    auto end_tile = board[x2][y2];
    if(is_correct_move(x1, y1, x2, y2))
    {
        if(board[x1][y1].getPiece().getPieceType() == PieceType::KING) {
            if(board[x1][y1].getPiece().getColor() == PieceColor::WHITE) white_king_moved = true;
            else black_king_moved = true;
        }
        if(curr_enpassant == enpassant) {
            enpassant = {-1, -1};
        }
        //If capture or pawn move then 50 move rule has zero moves
        if(board[x2][y2].getHasPC() || board[x1][y1].getPiece().getPieceType() == PieceType::PAWN) numHalfMov50 = 0;
        else {
            ++numHalfMov50;
        }
        if(board[x1][y1].getPiece().getColor() == PieceColor::BLACK) ++numMov;
        board[x2][y2].setHasPC(true);
        board[x2][y2].setPiece(board[x1][y1].getPiece());
        board[x1][y1].setHasPC(false);

        //Can't make a non-blocking move while in check
        if(start_tile.getPiece().getColor() == PieceColor::WHITE) {
            if(is_white_check()) { //Undo move
                board[x1][y1] = start_tile;
                board[x2][y2] = end_tile;
                return false;
            }
        } else if(is_black_check()) { //Undo move
            board[x1][y1] = start_tile;
            board[x2][y2] = end_tile;
            return false;
        }

        //Check if any are in check after making a move
        if(is_white_check()) white_check = true;
        else white_check = false;

        if(is_black_check()) black_check = true;
        else black_check = false;

        if(start_tile.getPiece().getPieceType() == PieceType::PAWN) {
            if(start_tile.getPiece().getColor() == PieceColor::WHITE) {
                if(x1 == 6 && x2 == 4) {
                    enpassant = {x1-1, y1};
                } else if(x1 == 1 && x2 == 0) { //Promoting
                    std::cout << "Promote to:\n1. Queen\n2. Knight\n3. Rook\n4. Bishop\n";

                    int choice;
                    std::cin >> choice;
                    do {
                        if(choice == 1) {
                            board[x2][y2].setPiece(Piece(PieceColor::WHITE, PieceType::QUEEN));
                        } else if(choice == 2) {
                            board[x2][y2].setPiece(Piece(PieceColor::WHITE, PieceType::KNIGHT));
                        } else if(choice == 3) {
                            board[x2][y2].setPiece(Piece(PieceColor::WHITE, PieceType::ROOK));
                        } else if(choice == 4) {
                            board[x2][y2].setPiece(Piece(PieceColor::WHITE, PieceType::BISHOP));
                        } else {
                            std::cout << "Input a proper number from 1-4: ";
                            std::cin >> choice;
                        }
                    } while(choice < 1 || choice > 4);
                }
            } else if(x1 == 1 && x2 == 3) {
                enpassant = {x1+1, y1};
            } else if(x1 == 6 && x2 == 7) {
                std::cout << "Promote to:\n1. Queen\n2. Knight\n3. Rook\n4. Bishop\n";

                int choice;
                std::cin >> choice;
                do {
                    if(choice == 1) {
                        board[x2][y2].setPiece(Piece(PieceColor::BLACK, PieceType::QUEEN));
                    } else if(choice == 2) {
                        board[x2][y2].setPiece(Piece(PieceColor::BLACK, PieceType::KNIGHT));
                    } else if(choice == 3) {
                        board[x2][y2].setPiece(Piece(PieceColor::BLACK, PieceType::ROOK));
                    } else if(choice == 4) {
                        board[x2][y2].setPiece(Piece(PieceColor::BLACK, PieceType::BISHOP));
                    } else {
                        std::cout << "Input a proper number from 1-4: ";
                        std::cin >> choice;
                    }
                } while(choice < 1 || choice > 4);
            }
        }
        return true;
    }
    else return false;
}

bool isEqual(int x1, int y1, int x2, int y2)
{
    if(x2 == x1 && y1 == y2) return true;
    return false;
}

std::vector<std::pair<int, int>> Board::generate_all_white() {
    std::vector<std::pair<int, int>> res;
    for(int i = 0; i<(int)board.size(); ++i) {
        for(int j = 0; j<(int)board[i].size(); ++j) {
            for(int k = 0; k<(int)board.size(); ++k) {
                for(int a = 0; a<(int)board[k].size(); ++a) {
                    if(k == i && a == j) continue;
                    if(board[i][j].getHasPC() && board[i][j].getPiece().getColor() == PieceColor::WHITE) {
                        if(is_correct_move(i, j, k, a)) 
                            res.push_back({k, a});
                    }
                }
            }
        }
    }
    return res;
}

std::vector<std::pair<int, int>> Board::generate_all_black() {
    std::vector<std::pair<int, int>> res;
    for(int i = 0; i<(int)board.size(); ++i) {
        for(int j = 0; j<(int)board[i].size(); ++j) {
            for(int k = 0; k<(int)board.size(); ++k) {
                for(int a = 0; a<(int)board[k].size(); ++a) {
                    if(k == i && a == j) continue;
                    if(board[i][j].getHasPC() && board[i][j].getPiece().getColor() == PieceColor::BLACK) {
                        if(is_correct_move(i, j, k, a)) 
                            res.push_back({k, a});
                    }
                }
            }
        }
    }
    return res;
}

bool Board::is_white_check() {
    auto poss_moves = generate_all_black();
    for(auto &mov : poss_moves) {
        if(board[mov.first][mov.second].getHasPC() && board[mov.first][mov.second].getPiece().getPieceType() == PieceType::KING &&
        board[mov.first][mov.second].getPiece().getColor() == PieceColor::WHITE) return true;
    }
    return false;
}

bool Board::is_black_check() {
    auto poss_moves = generate_all_white();
    for(auto &mov : poss_moves) {
        if(board[mov.first][mov.second].getHasPC() && board[mov.first][mov.second].getPiece().getPieceType() == PieceType::KING &&
        board[mov.first][mov.second].getPiece().getColor() == PieceColor::BLACK) return true;
    }
    return false;
}

void Board::check_castle() {
    if(white_king_moved || white_check) {
        WK = false;
        WQ = false;
    } else {
        auto poss_moves = generate_all_black();
        std::vector<std::pair<int, int>> check = { std::make_pair(7, 3), std::make_pair(7, 2) };
        for(auto &mov : poss_moves) {
            for(auto &m : check) {
                if(mov == m) {
                    WQ = false;
                    break;
                }
            }
        }
        check = {std::make_pair(7, 5), std::make_pair(7, 6)};
        for(auto &mov : poss_moves) {
            for(auto &m : check) {
                if(mov == m) {
                    WK = false;
                    break;
                }
            }
        }
    }

    if(black_king_moved || black_check) {
        bk = false;
        bq = false;
    } else {
        auto poss_moves = generate_all_white();
        auto check = { std::make_pair(0, 3), std::make_pair(0, 2) };
        for(auto &mov : poss_moves) {
            for(auto &m : check) {
                if(mov == m) {
                    bq = false;
                    break;
                }
            }
        }
        check = {std::make_pair(0, 5), std::make_pair(0, 6)};
        for(auto &mov : poss_moves) {
            for(auto &m : check) {
                if(mov == m) {
                    bk = false;
                    break;
                }
            }
        }
    }
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
            for(int i = x1+1; i<(int)board.size(); ++i)
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
            for(int i = y1+1; i<(int)board[0].size(); ++i)
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
        for(int i = 1; i<(int)board.size(); ++i)
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
        for(int i = 1; i<(int)board.size(); ++i)
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
        for(int i = 1; i<(int)board.size(); ++i)
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
        for(int i = 1; i<(int)board.size(); ++i)
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
            for(int i = x1+1; i<(int)board.size(); ++i)
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
            for(int i = y1+1; i<(int)board[0].size(); ++i)
            {
                if(board[x1][i].getHasPC())
                {
                    if(board[x1][i].getPiece().getColor() != board[x1][y1].getPiece().getColor() && i == y2) return true;
                    return false;
                }
                else if(i == y2) return true;
            }
        }
        for(int i = 1; i<(int)board.size(); ++i)
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
        for(int i = 1; i<(int)board.size(); ++i)
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
        for(int i = 1; i<(int)board.size(); ++i)
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
        for(int i = 1; i<(int)board.size(); ++i)
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
            if(x2 == x1+1 && y1 == y2){//Normal pawn move
                if(board[x2][y2].getHasPC() == false) return true;
                return false;
            }
            //Move two steps forward
            else if(x1 == 1 && x2 == x1+2 && y1 == y2 && board[x1+1][y1].getHasPC() == false && board[x2][y2].getHasPC() == false) {
                return true;
            }
            //Take diagonally
            else if(x2 == x1+1 && y2 == y1-1 && board[x2][y2].getHasPC() && board[x2][y2].getPiece().getColor() != board[x1][y1].getPiece().getColor()) return true;
            else if(x2 == x1+1 && y2 == y1+1 && board[x2][y2].getHasPC() && board[x2][y2].getPiece().getColor() != board[x1][y1].getPiece().getColor()) return true;
        }
        //For White
        else {
            if(x2 == x1-1 && y1 == y2){
                if(board[x2][y2].getHasPC() == false) return true;
                return false;
            }
            else if(x1 == 6 && x2 == x1-2 && y1 == y2 && board[x1-1][y1].getHasPC() == false && board[x2][y2].getHasPC() == false) {
                return true;
            }
            else if(x2 == x1-1 && y2 == y1-1 && board[x2][y2].getHasPC() && board[x2][y2].getPiece().getColor() != board[x1][y1].getPiece().getColor()) return true;
            else if(x2 == x1-1 && y2 == y1+1 && board[x2][y2].getHasPC() && board[x2][y2].getPiece().getColor() != board[x1][y1].getPiece().getColor()) return true;
        }
        return false;
    }
    return false;
}

bool Board::is_valid_move(int x, int y)
{
    if(x >= 0 && x <(int)board.size() && y >= 0 && y <(int)board[0].size()) return true;
    return false;
}

std::string Board::chessNot(const std::pair<int, int> &pos) {
    std::string res = "";
    res += 'a'+pos.second;
    res += '1'+(7-pos.first);
    return res;
}

std::pair<int, int> Board::fromChessNot(std::string str) {
    if(str.size() < 2) return {-1, -1};
    return {7-(str[1]-'1'), str[0]-'a'};
}

std::string Board::to_fen() {
    std::string fen = "";
    //Board notation
    for(int i = 0; i<(int)board.size(); ++i) {
        for(int j = 0; j<(int)board[i].size(); ++j) {
            //If empty square
            if(board[i][j].getHasPC() == false) {
                //Count length of consec empty squares
                int len = 0;
                while(j < (int)board[i].size()) {
                    if(board[i][j].getHasPC() == false) {
                        ++len;
                    }
                    else break;
                    ++j;
                }
                --j;
                fen += '0'+len;
            } else {
                fen += board[i][j].getPiece().getChar();
            }
        }
        if(i != (int)board.size()-1) {
            fen += "/";
        }
    }

    //Turn
    fen += " ";
    if(turn == Turn::WHITE) {
        fen += "w ";
    } else fen += "b ";
    
    //Castling
    if(!get_white_king_castle() && !get_white_queen_castle() && !get_black_king_castle() && !get_black_queen_castle()) {
        fen += "-";
    }
    if(get_white_king_castle()) fen += "K";
    if(get_white_queen_castle()) fen += "Q";
    if(get_black_king_castle()) fen += "k";
    if(get_black_queen_castle()) fen += "q";
    fen += " ";

    //Enpassant
    if(enpassant == std::make_pair(-1, -1)) {
        fen += "- ";
    } else {
        fen += chessNot(enpassant);
        fen += " ";
    }

    //50 move rule
    fen += std::to_string(numHalfMov50);
    fen += " ";

    //number of moves
    fen += std::to_string(numMov);
    return fen;
}

