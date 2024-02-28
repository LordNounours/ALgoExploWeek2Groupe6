#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>
#include <cmath>
#include <unordered_map>

// Classe représentant le plateau de jeu de Tic Tac Toe
class TicTacToeBoard {
public:
    std::vector<std::vector<int>> board; // Plateau de jeu
    int turn; // Tour du joueur actuel

    // Constructeur de la classe
    TicTacToeBoard(std::vector<std::vector<int>> board = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, int turn = 1)
        : board(board), turn(turn) {}

    // Méthode pour effectuer un mouvement sur le plateau
    TicTacToeBoard make_move(int x, int y) const {
        std::vector<std::vector<int>> new_board = board;
        new_board[x][y] = turn;
        return TicTacToeBoard(new_board, 3 - turn); // Correction du calcul du tour
    }

    // Méthode pour convertir le plateau en une chaîne de caractères pour l'affichage
    std::string to_string() const {
        std::string s = (turn == 1 ? "X" : "O");
        for (const auto& row : board) {
            s += "\n";
            for (int cell : row) {
                s += (cell == 1 ? "X" : cell == 2 ? "O" : " ");
            }
        }
        return s;
    }

    // Méthode pour générer tous les plateaux possibles après un mouvement
    std::vector<TicTacToeBoard> getChildren() const {
        std::vector<TicTacToeBoard> moves;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (board[i][j] == 0) {
                    moves.push_back(make_move(i, j));
                }
            }
        }
        return moves;
    }

    // Méthode pour vérifier si le jeu est terminé
    int isFinal() const {
        bool found_zero = false;
        for (const auto& row : board) {
            for (int cell : row) {
                if (cell == 0) {
                    found_zero = true;
                    break;
                }
            }
            if (!found_zero) break;
        }
        if (!found_zero) return 0;

        for (int i = 0; i < 3; ++i) {
            if (board[i][0] == board[i][1] && board[i][1] == board[i][2] && board[i][0] != 0) {
                return board[i][0];
            }
            if (board[0][i] == board[1][i] && board[1][i] == board[2][i] && board[0][i] != 0) {
                return board[0][i];
            }
        }
        if (board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != 0) {
            return board[0][0];
        }
        if (board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[0][2] != 0) {
            return board[0][2];
        }

        return -1;
    }

    // Méthode pour comparer deux instances de TicTacToeBoard
    bool operator==(const TicTacToeBoard& other) const {
        return board == other.board && turn == other.turn;
    }

    // Méthode pour obtenir un hash d'une instance de TicTacToeBoard
    size_t hash() const {
        return std::hash<std::string>()(to_string());
    }
};

// Liste pour stocker les plateaux de jeu chargés
std::vector<TicTacToeBoard> boards;

// Fonction pour charger les plateaux de jeu à partir d'un fichier
void loadBoardsFromFile(const std::string& filename) {
    std::ifstream dataset(filename);
    std::string line;
    while (std::getline(dataset, line)) {
        std::vector<std::vector<int>> board = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
        int turn = 1;
        if (line[0] == 'X') turn = 2;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (line[1 + 3 * i + j] == 'X') {
                    board[i][j] = 1;
                } else if (line[1 + 3 * i + j] == 'O') {
                    board[i][j] = 2;
                }
            }
        }
        // Afficher la ligne parsée
        std::cout << "Ligne parsée : " << line << std::endl;
        boards.push_back(TicTacToeBoard(board, turn));
    }
}

// Fonction pour implémenter l'algorithme Minimax
std::pair<int, int> minimax(const TicTacToeBoard& board, int playerCurrent, int turn) {
    int v = board.isFinal();
    if (v != -1) {
        return {(v == turn ? 1 : -1), 0};
    }

    if (playerCurrent == turn) {
        std::pair<int, int> m = {-99999, -1};
        for (int i = 0; i < board.getChildren().size(); ++i) {
            auto rslt = minimax(board.getChildren()[i], playerCurrent, turn);
            if (rslt.first > m.first) {
                m = {rslt.first, i};
            }
        }
        return m;
    } else {
        std::pair<int, int> m = {99999, -1};
        for (int i = 0; i < board.getChildren().size(); ++i) {
            auto rslt = minimax(board.getChildren()[i], playerCurrent, turn);
            if (rslt.first < m.first) {
                m = {rslt.first, i};
            }
        }
        return m;
    }
}

int main() {
    std::string filename = "dataset.txt";
    loadBoardsFromFile(filename);

    std::cout << boards[0].to_string() << "\n\n";
    std::cout << minimax(boards[0], boards[0].turn, boards[0].turn).second << std::endl;

    clock_t start = clock();
    auto v = minimax(TicTacToeBoard(), 1, 1);
    clock_t end = clock();

    std::cout << "Temps d'exécution : " << (double(end - start) / CLOCKS_PER_SEC) << " secondes" << std::endl;

    int total_games = 0;
    int wins = 0;
    int losses = 0;
    int draws = 0;

    for (const auto& initial_board : boards) {
        auto result = minimax(initial_board, initial_board.turn, initial_board.turn);
        total_games += 1;
        if (result.first == 1) {
            wins += 1;
        } else if (result.first == -1) {
            losses += 1;
        } else {
            draws += 1;
        }
    }

    double win_percentage = (double)wins / total_games * 100;
    double loss_percentage = (double)losses / total_games * 100;
    double draw_percentage = (double)draws / total_games * 100;

    std::cout << "Pourcentage de victoires : " << win_percentage << "%" << std::endl;
    std::cout << "Pourcentage de défaites : " << loss_percentage << "%" << std::endl;
    std::cout << "Pourcentage de matchs nuls : " << draw_percentage << "%" << std::endl;

    return 0;
}

