#include <iostream>
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <sys/wait.h>
#include <string>

using namespace std;

std::string func() {
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
        Commands = "position fen rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1\n";
        if(write(toStockfish[1], Commands, strlen(Commands)) == -1) {
            std::cout << "Error writing to stockfish!" << std::endl;
            return "";
        }
        Commands = "go movetime\n";
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
        //std::cout << "Status: " << status << std::endl;
    }
    return res;
}

int main() {
    std::string res = func();
    cout << res << endl;
    
    return 0;
}
