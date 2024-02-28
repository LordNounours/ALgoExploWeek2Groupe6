import numpy as np
import time

# Classe représentant le plateau de jeu de Tic Tac Toe
class TicTacToeBoard:
    # Constructeur de la classe
    def __init__(self, board=None, turn=1):
        # Si aucun plateau n'est fourni, initialise un nouveau plateau vide
        if board is None:
            self.board = np.zeros((3, 3), dtype=int)
        else:
            self.board = np.array(board)
        # Initialise le tour du joueur actuel
        self.turn = turn

    # Méthode pour effectuer un mouvement sur le plateau
    def make_move(self, x, y):
        # Crée une copie du plateau actuel
        new_board = np.array(self.board)
        # Place le symbole du joueur actuel à la position spécifiée
        new_board[x][y] = self.turn
        # Retourne un nouvel objet TicTacToeBoard avec le nouveau plateau et le tour suivant
        return TicTacToeBoard(new_board, 3 - self.turn)

    # Méthode pour convertir le plateau en une chaîne de caractères pour l'affichage
    def to_string(self):
        # Détermine le symbole du joueur actuel
        s = 'X' if self.turn == 1 else 'O'
        # Convertit le plateau en une chaîne de caractères
        for row in self.board:
            s += '\n' + ''.join(['X' if cell == 1 else 'O' if cell == 2 else ' ' for cell in row])
        return s

    # Méthode pour générer tous les plateaux possibles après un mouvement
    def getChildren(self):
        moves = []
        for i in range(3):
            for j in range(3):
                if self.board[i][j] == 0:
                    moves.append(self.make_move(i, j))
        return moves

    # Méthode pour vérifier si le jeu est terminé
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

    # Méthode pour comparer deux instances de TicTacToeBoard
    def __eq__(self, other):
        return np.array_equal(self.board, other.board) and self.turn == other.turn

    # Méthode pour obtenir un hash d'une instance de TicTacToeBoard
    def __hash__(self):
        return hash(str(self.board) + str(self.turn))

# Liste pour stocker les plateaux de jeu chargés
boards = []

# Fonction pour charger les plateaux de jeu à partir d'un fichier
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
            print("Ligne parsée :", line.strip())
            boards.append(TicTacToeBoard(board, turn))

# Fonction pour implémenter l'algorithme Minimax
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

# Nom du fichier contenant les plateaux de jeu
filename = "dataset.txt"
# Charge les plateaux de jeu à partir du fichier
loadBoardsFromFile(filename)

# Affiche le premier plateau de jeu chargé
print(boards[0].to_string(), "\n")
# Affiche le meilleur mouvement pour le premier plateau de jeu
print(minimax(boards[0], boards[0].turn, boards[0].turn)[1])

# Mesure le temps d'exécution de l'algorithme Minimax sur un nouveau plateau
start = time.time()
v = minimax(TicTacToeBoard(), 1, 1)
end = time.time()

# Affiche le temps d'exécution
print("Temps d'exécution :", end - start)

# Initialise les compteurs pour les statistiques
total_games = 0
wins = 0
losses = 0
draws = 0

# Exécute l'algorithme Minimax sur chaque plateau chargé et compte les résultats
for initial_board in boards:
    result = minimax(initial_board, initial_board.turn, initial_board.turn)
    total_games += 1
    if result[0] == 1:
        wins += 1
    elif result[0] == -1:
        losses += 1
    else:
        draws += 1

# Calcule les pourcentages de victoires, défaites et matchs nuls
win_percentage = (wins / total_games) * 100
loss_percentage = (losses / total_games) * 100
draw_percentage = (draws / total_games) * 100

# Affiche les statistiques
print("Pourcentage de victoires :", win_percentage, "%")
print("Pourcentage de défaites :", loss_percentage, "%")
print("Pourcentage de matchs nuls :", draw_percentage, "%")

