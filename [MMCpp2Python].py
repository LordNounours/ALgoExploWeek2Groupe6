import numpy as np
import time

class TicTacToeBoard:
    def __init__(self, board=None, turn=1):
        if board is None:
            self.board = np.zeros((3, 3), dtype=int)
        else:
            self.board = np.array(board)
        self.turn = turn

    def make_move(self, x, y):
        new_board = np.array(self.board)
        new_board[x][y] = self.turn
        return TicTacToeBoard(new_board, 3 - self.turn)

    def to_string(self):
        s = 'X' if self.turn == 1 else 'O'
        for row in self.board:
            s += '\n' + ''.join(['X' if cell == 1 else 'O' if cell == 2 else ' ' for cell in row])
        return s

    def getChildren(self):
        moves = []
        for i in range(3):
            for j in range(3):
                if self.board[i][j] == 0:
                    moves.append(self.make_move(i, j))
        return moves

    def isFinal(self):
        found_zero = False
        for row in self.board:
            if 0 in row:
                found_zero = True
                break
        if not found_zero:
            return 0

        for i in range(3):
            if self.board[i][0] == self.board[i][1] == self.board[i][2] and self.board[i][0] != 0:
                return self.board[i][0]
            if self.board[0][i] == self.board[1][i] == self.board[2][i] and self.board[0][i] != 0:
                return self.board[0][i]
        if self.board[0][0] == self.board[1][1] == self.board[2][2] and self.board[0][0] != 0:
            return self.board[0][0]
        if self.board[0][2] == self.board[1][1] == self.board[2][0] and self.board[0][2] != 0:
            return self.board[0][2]
        return -1

    def __eq__(self, other):
        return np.array_equal(self.board, other.board) and self.turn == other.turn

    def __hash__(self):
        return hash(str(self.board) + str(self.turn))

boards = []

def loadBoardsFromFile(filename):
    with open(filename, 'r') as dataset:
        for line in dataset.readlines():
            board = [[0, 0, 0], [0, 0, 0], [0, 0, 0]]
            turn = 1 if line[0] == 'X' else 2
            for i in range(3):
                for j in range(3):
                    if line[1 + 3 * i + j] == 'X':
                        board[i][j] = 1
                    elif line[1 + 3 * i + j] == 'O':
                        board[i][j] = 2
            # Afficher la ligne parsée
            print("Ligne parsée :", line.strip())
            boards.append(TicTacToeBoard(board, turn))

def minimax(board, playerCurrent, turn):
    v = board.isFinal()
    if v != -1:
        return (1 if v == turn else -1 if v != 0 else 0, 0)

    if playerCurrent == turn:
        m = (-99999, None)
        for i, child in enumerate(board.getChildren()):
            rslt = minimax(child, playerCurrent, turn)
            if rslt[0] > m[0]:
                m = (rslt[0], i)
        return m
    else:
        m = (99999, None)
        for i, child in enumerate(board.getChildren()):
            rslt = minimax(child, playerCurrent, turn)
            if rslt[0] < m[0]:
                m = (rslt[0], i)
        return m

filename = "dataset.txt"
loadBoardsFromFile(filename)

print(boards[0].to_string(), "\n")
print(minimax(boards[0], boards[0].turn, boards[0].turn)[1])

start = time.time()
v = minimax(TicTacToeBoard(), 1, 1)
end = time.time()

print("Temps d'exécution :", end - start)

total_games = 0
wins = 0
losses = 0
draws = 0

for initial_board in boards:
    result = minimax(initial_board, initial_board.turn, initial_board.turn)
    total_games += 1
    if result[0] == 1:
        wins += 1
    elif result[0] == -1:
        losses += 1
    else:
        draws += 1

win_percentage = (wins / total_games) * 100
loss_percentage = (losses / total_games) * 100
draw_percentage = (draws / total_games) * 100

print("Pourcentage de victoires :", win_percentage, "%")
print("Pourcentage de défaites :", loss_percentage, "%")
print("Pourcentage de matchs nuls :", draw_percentage, "%")
