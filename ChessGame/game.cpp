#include "game.h"

Game::Game()
{}

Game::Game(const Game &g) :board(g.board)
{}

Game::Game(Game &&g) :board(g.board)
{}

void Game::GameLoop()
{
    while(true)
    {
        std::cout << board;
        std::cout << board.to_fen() << std::endl;
        if(board.get_white_check() || board.get_black_check()) {
            std::cout << "CHECK!" << std::endl;
        }
        std::cout << "Enter move: ";
        int x1, y1, x2, y2;
        std::cin >> x1 >> y1 >> x2 >> y2;
        std::pair<int, int> pr1 = std::make_pair(x1, y1);
        std::pair<int, int> pr2 = std::make_pair(x2, y2);
        std::cout << board.chessNot(pr1) << " " << board.chessNot(pr2) << std::endl;
        if(board.make_move(x1, y1, x2, y2))
        {
            board.switch_turn();
        } else {
            std::cout << "Invalid Move!" << std::endl;
        }
    }
}