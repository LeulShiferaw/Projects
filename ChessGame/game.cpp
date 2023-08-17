#include "game.h"
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

//Calculates the best move from stockfish for the current board position
std::string Game::bestMoveStockfish() {
    // Create pipes for communication
    int toStockfish[2];  // Pipe for sending commands to Stockfish
    int fromStockfish[2];  // Pipe for receiving responses from Stockfish
    std::string res;

    if (pipe(toStockfish) == -1 || pipe(fromStockfish) == -1) {
        perror("pipe");
        return "";
    }

    // Fork to create a subprocess
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        return "";
    } else if (pid == 0) {
        // Child process: Start Stockfish
        close(toStockfish[1]);
        close(fromStockfish[0]);
        dup2(toStockfish[0], STDIN_FILENO);
        dup2(fromStockfish[1], STDOUT_FILENO);
        close(toStockfish[0]);
        close(fromStockfish[1]);

        // Execute Stockfish directly (assuming it's in your PATH)
        execlp("stockfish", "stockfish", nullptr);


        perror("execlp");
        return "";
    } else {
        // Parent process: Communicate with Stockfish
        close(toStockfish[0]);
        close(fromStockfish[1]);

        // Send UCI commands to Stockfish
        const char* Commands = "uci\n";
        if(write(toStockfish[1], Commands, strlen(Commands)) == -1) {
            std::cout << "Error writing to stockfish!" << std::endl;
            return "";
        }
        std::string fen_commands = "position fen " + board.to_fen() + "\n";
        if(write(toStockfish[1], fen_commands.c_str(), fen_commands.size()) == -1) {
            std::cout << "Error writing to stockfish!" << std::endl;
            return "";
        }
        Commands = "go movetime 6000\n";
        if(write(toStockfish[1], Commands, strlen(Commands)) == -1) {
            std::cout << "Error writing to stockfish!" << std::endl;
            return "";
        }

        // Read and discard engine output until "uciok" is encountered
        char buffer[2000];
        ssize_t bytesRead;
        while ((bytesRead = read(fromStockfish[0], buffer, sizeof(buffer))) > 0) {
            buffer[bytesRead] = '\0';
            // Check if the "bestmove" line is present
            char* bestmovePosition = strstr(buffer, "bestmove");
            if (bestmovePosition != nullptr) {
                char bestmove[128];  // Adjust the size as needed
                if (sscanf(bestmovePosition, "bestmove %s", bestmove) == 1) {
                    //printf("Best Move: %s\n", bestmove);
                    res = bestmove;
                }
                break;  // Exit the loop after processing the bestmove line
            } 
        } 

        close(toStockfish[1]);  // Close the write end
        close(fromStockfish[0]);  // Close the read end

        // Wait for the child process to exit
        int status;
        waitpid(pid, &status, 0);
        kill(pid, SIGKILL);
        //std::cout << "Status: " << status << std::endl;
    }
    return res;
}



Game::Game()
{}

Game::Game(const Game &g) :board(g.board)
{}

Game::Game(Game &&g) :board(g.board)
{}

void Game::GameLoop()
{
    std::cout << "Do you want single(1) or multiplayer(2): ";

    int ch;
    std::cin >> ch;

    do {
        if(ch == 1) {
            SinglePlayer();
        } else MultiPlayer();
    } while(ch < 1 || ch > 2);

    
}

void Game::SinglePlayer() {
    while(true)
    {
        std::cout << board;
        std::cout << board.to_fen() << std::endl;
        if(board.get_white_check() || board.get_black_check()) {
            std::cout << "CHECK!" << std::endl;
        }

        if(board.getTurn() == Turn::WHITE) {
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
        } else {
            auto bestMove = bestMoveStockfish();
            cout << "Engine Move: " << bestMove << endl;
            auto mov1 = board.fromChessNot(bestMove.substr(0, 2));
            auto mov2 = board.fromChessNot(bestMove.substr(2, 2));
            if(board.make_move(mov1.first, mov1.second, mov2.first, mov2.second)) {
                board.switch_turn();
            } else {
                cout << "Invalid Engine Move!" << endl;
            }
        }
    }
}

void Game::MultiPlayer() {
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