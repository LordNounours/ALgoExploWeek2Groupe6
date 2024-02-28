import sys
import time
EMPTY = 0
PLAYER_X = 1
PLAYER_O = 2
DRAW = 0
PLAYER_X_WINS = 1
PLAYER_O_WINS = -1

board = [[0, 0, 0], [0, 0, 0], [0, 0, 0]]
currentPlayer = PLAYER_X

def load_game_boards(filename):
    game_boards = []
    with open(filename, 'r') as file:
        for line in file:
            game_board = [[0, 0, 0], [0, 0, 0], [0, 0, 0]]
            turn = PLAYER_O if line[0] == 'O' else PLAYER_X
            for i in range(3):
                for j in range(3):
                    cell = line[i * 3 + j + 1]
                    game_board[i][j] = EMPTY if cell == ' ' else PLAYER_X if cell == 'X' else PLAYER_O
            game_boards.append(game_board)
    return game_boards

def minimax(board, depth, is_maximizing_player):
    result = check_win(board)
    if result != 0:
        return result

    if is_maximizing_player:
        best_score = float('-inf')
        for i in range(3):
            for j in range(3):
                if board[i][j] == EMPTY:
                    board[i][j] = PLAYER_X
                    score = minimax(board, depth + 1, False)
                    board[i][j] = EMPTY
                    best_score = max(score, best_score)
        return best_score
    else:
        best_score = float('inf')
        for i in range(3):
            for j in range(3):
                if board[i][j] == EMPTY:
                    board[i][j] = PLAYER_O
                    score = minimax(board, depth + 1, True)
                    board[i][j] = EMPTY
                    best_score = min(score, best_score)
        return best_score

def check_win(board):
    for i in range(3):
        if board[i][0] == board[i][1] == board[i][2] != 0:
            return PLAYER_X_WINS if board[i][0] == PLAYER_X else PLAYER_O_WINS
    for i in range(3):
        if board[0][i] == board[1][i] == board[2][i] != 0:
            return PLAYER_X_WINS if board[0][i] == PLAYER_X else PLAYER_O_WINS
    if board[0][0] == board[1][1] == board[2][2] != 0:
        return PLAYER_X_WINS if board[0][0] == PLAYER_X else PLAYER_O_WINS
    if board[0][2] == board[1][1] == board[2][0] != 0:
        return PLAYER_X_WINS if board[0][2] == PLAYER_X else PLAYER_O_WINS
    return DRAW

if __name__ == "__main__":
    game_boards = load_game_boards("dataset.txt")
    start = time.time()
    for game_board in game_boards:
        board[:] = game_board
        result = minimax(board, 0, True)
        print("Résultat pour le plateau initial :", result)
    end = time.time()
    print("Temps d'exécution :", end - start)