import subprocess

def get_best_move(fen_position, skill_level=20):
    stockfish_path = "stockfish"  # Replace with the actual path to your Stockfish executable
    command = [stockfish_path, f"setoption name Skill Level value {skill_level}", "position startpos", "go depth 1"]
    
    process = subprocess.Popen(
        command,
        universal_newlines=True,
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
    )
    
    process.stdin.write("isready\n")
    process.stdin.flush()
    while True:
        line = process.stdout.readline().strip()
        if line == "readyok":
            break
        elif len(line) > 0:
            print(line)
    
    process.stdin.write("stop\n")
    process.stdin.flush()
    while True:
        line = process.stdout.readline().strip()
        if line.startswith("bestmove"):
            best_move = line.split()[1]
            break
    
    process.stdin.close()
    process.stdout.close()
    process.stderr.close()
    process.wait()
    
    return best_move

if __name__ == "__main__":
    fen_position = "position startpos"
    best_move = get_best_move(fen_position)
    print("Best move:", best_move)
