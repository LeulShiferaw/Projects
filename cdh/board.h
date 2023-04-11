#pragma once

#include <vector>
#include "tile.h"

class Board
{
    private:
        std::vector<std::vector<Tile>> board;
    public:
        Board();
        Board(Board &);
        Board(Board &&);
};