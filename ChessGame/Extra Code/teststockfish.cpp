#include <iostream>
#include <cstdio>
#include <cstring>

int main() {
    FILE* stockfishProcess = popen("stockfish 2>/dev/null", "w");

    if (stockfishProcess == nullptr) {
        std::cerr << "Error starting Stockfish process." << std::endl;
        return 1;
    }

    // Send UCI commands
    fprintf(stockfishProcess, "uci\nposition startpos\ngo movetime 3000");
    fflush(stockfishProcess);

    // Read and discard engine output until "uciok" is encountered
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), stockfishProcess) != nullptr) {
        if (std::strcmp(buffer, "uciok\n") == 0) {
            break;
        }
    }

    // ... Send more UCI commands and receive responses ...

    // Close the Stockfish process
    pclose(stockfishProcess);

    return 0;
}
