#pragma once

#include "board.h"

class Game {
    private:
        Board board;
    public:
        Game();
        Game(const Game &);
        Game(Game &&);

        void GameLoop();
        void SinglePlayer();
        void MultiPlayer();
        std::string bestMoveStockfish();
};