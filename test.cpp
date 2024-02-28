#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <chrono>

class TicTacToeBoard {
private:
    std::array<std::array<int, 3>, 3> board;
    int turn;

public:
    TicTacToeBoard(std::array<std::array<int, 3>, 3> board = {{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}}}, int turn = 1) : board(board), turn(turn) {}

    int getTurn() const {
        return turn;
    }

    TicTacToeBoard make_move(int x, int y) const {
        auto new_board = board;
        new_board[x][y] = turn;
        return TicTacToeBoard(new_board, 3 - turn);
    }

    void printToFile(std::ofstream& file) const {
        file << *this << std::endl;
    }

    friend std::ostream& operator<<(std::ostream& os, const TicTacToeBoard& board) {
        char turn_char = (board.turn == 1) ? 'X' : 'O';
        os << turn_char;
        for (const auto& row : board.board) {
            os << std::endl;
            for (int cell : row) {
                os << (cell == 1 ? 'X' : (cell == 2 ? 'O' : ' '));
            }
        }
        return os;
    }

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

    int isFinal() const {
        bool found_zero = false;
        for (const auto& row : board) {
            for (int cell : row) {
                if (cell == 0) {
                    found_zero = true;
                    break;
                }
            }
            if (found_zero) {
                break;
            }
        }
        if (!found_zero) {
            return 0;
        }

        for (int i = 0; i < 3; ++i) {
            if (board[i][0] == board[i][1] == board[i][2] != 0) {
                return board[i][0];
            }
            if (board[0][i] == board[1][i] == board[2][i] != 0) {
                return board[0][i];
            }
        }
        if (board[0][0] == board[1][1] == board[2][2] != 0) {
            return board[0][0];
        }
        if (board[0][2] == board[1][1] == board[2][0] != 0) {
            return board[0][2];
        }

        return -1;
    }

    bool operator==(const TicTacToeBoard& other) const {
        return board == other.board && turn == other.turn;
    }

    std::size_t hash() const {
        std::hash<std::string> hash_fn;
        return hash_fn(std::to_string(turn) + std::to_string(board[0][0]) + std::to_string(board[0][1]) + std::to_string(board[0][2]) +
                       std::to_string(board[1][0]) + std::to_string(board[1][1]) + std::to_string(board[1][2]) +
                       std::to_string(board[2][0]) + std::to_string(board[2][1]) + std::to_string(board[2][2]));
    }
};

std::vector<TicTacToeBoard> loadBoardsFromFile(const std::string& filename) {
    std::ifstream dataset(filename);
    if (!dataset.is_open()) {
        std::cerr << "Error: Couldn't open file " << filename << std::endl;
        return {};
    }

    std::vector<TicTacToeBoard> boards;
    std::string line;
    while (std::getline(dataset, line)) {
        std::array<std::array<int, 3>, 3> board = {{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}}};
        int turn = (line[0] == 'X') ? 1 : 2;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (line[1 + 3 * i + j] == 'X') {
                    board[i][j] = 1;
                } else if (line[1 + 3 * i + j] == 'O') {
                    board[i][j] = 2;
                }
            }
        }
        std::cout << "Parsed line: " << line << std::endl;
        boards.push_back(TicTacToeBoard(board, turn));
    }

    return boards;
}

std::pair<int, int> minimax(const TicTacToeBoard& board, int playerCurrent, int turn) {
    int v = board.isFinal();
    if (v != -1) {
        return {(v == turn) ? 1 : ((v != 0) ? -1 : 0), 0};
    }

    if (playerCurrent == turn) {
        std::pair<int, int> m = {-99999, 0};
        int i = 0;
        for (const auto& child : board.getChildren()) {
            auto result = minimax(child, playerCurrent, turn);
            if (result.first > m.first) {
                m = {result.first, i};
            }
            ++i;
        }
        return m;
    } else {
        std::pair<int, int> m = {99999, 0};
        int i = 0;
        for (const auto& child : board.getChildren()) {
            auto result = minimax(child, playerCurrent, turn);
            if (result.first < m.first) {
                m = {result.first, i};
            }
            ++i;
        }
        return m;
    }
}

int main() {
    std::vector<TicTacToeBoard> boards = loadBoardsFromFile("dataset.txt");

    std::cout << boards[0] << "\n";

    auto result = minimax(boards[0], boards[0].getTurn(), boards[0].getTurn());
    std::cout << result.second << std::endl;

    auto start = std::chrono::high_resolution_clock::now();
    result = minimax(TicTacToeBoard(), 1, 1);
    auto end = std::chrono::high_resolution_clock::now();

    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " milliseconds" << std::endl;

    // int total_games = 0, wins = 0, losses = 0, draws = 0;
    // for (const auto& initial_board : boards) {
    //     auto result = minimax(initial_board, initial_board.getTurn(), initial_board.getTurn());
    //     total_games++;
    //     if (result.first == 1) {
    //         wins++;
    //     } else if (result.first == -1) {
    //         losses++;
    //     } else {
    //         draws++;
    //     }
    // }

    // double win_percentage = (wins * 100.0) / total_games;
    // double loss_percentage = (losses * 100.0) / total_games;
    // double draw_percentage = (draws * 100.0) / total_games;

    // std::cout << "Pourcentage de victoires : " << win_percentage << "%" << std::endl;
    // std::cout << "Pourcentage de défaites : " << loss_percentage << "%" << std::endl;
    // std::cout << "Pourcentage de matchs nuls : " << draw_percentage << "%" << std::endl;

    return 0;
}