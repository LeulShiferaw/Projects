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
        std::cout << "Enter move: ";
        int x1, y1, x2, y2;
        std::cin >> x1 >> y1 >> x2 >> y2;
        if(board.make_move(x1, y1, x2, y2))
        {
            board.switch_turn();
        } else {
            std::cout << "Invalid Move!" << std::endl;
        }
    }
}