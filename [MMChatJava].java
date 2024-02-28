import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

public class TicTacToe {

    private static final int EMPTY = 0;
    private static final int PLAYER_X = 1;
    private static final int PLAYER_O = 2;
    private static final int DRAW = 0;
    private static final int PLAYER_X_WINS = 1;
    private static final int PLAYER_O_WINS = -1;

    private static int[][] board;
    private static int currentPlayer;

    public static int i = 0;

    public static void main(String[] args) throws IOException {
        board = new int[3][3];
        currentPlayer = PLAYER_X;

        // Load game boards from the file
        List<int[][]> gameBoards = loadGameBoards("dataset.txt");

        // Start the timer
        long startTime = System.nanoTime();

        for (int[][] gameBoard : gameBoards) {
            System.arraycopy(gameBoard, 0, board, 0, 3);
            int result = minimax(board, 0, true);
            System.out.println("Result for the initial board: " + result);
            i++;
        }

        // Stop the timer
        long endTime = System.nanoTime();

        // Calculate the elapsed time in milliseconds
        long elapsedTime = (endTime - startTime) / 1_000_000;

        // Print the execution time
        System.out.println("Execution time: " + elapsedTime + " milliseconds");
    }

    private static List<int[][]> loadGameBoards(String filename) throws IOException {
        List<int[][]> gameBoards = new ArrayList<>();
        try (BufferedReader reader = new BufferedReader(new FileReader(filename))) {
            String line;
            while ((line = reader.readLine()) != null) {
                int[][] gameBoard = new int[3][3];
                int turn = line.charAt(0) == 'O' ? PLAYER_O : PLAYER_X;
                for (int i = 0; i < 3; i++) {
                    for (int j = 0; j < 3; j++) {
                        char cell = line.charAt(i * 3 + j + 1);
                        gameBoard[i][j] = cell == ' ' ? EMPTY : cell == 'X' ? PLAYER_X : PLAYER_O;
                    }
                }
                gameBoards.add(gameBoard);
            }
        }
        return gameBoards;
    }

    private static int minimax(int[][] board, int depth, boolean isMaximizingPlayer) {
        int result = checkWin(board);
        if (result != 0) {
            return result;
        }

        if (isMaximizingPlayer) {
            int bestScore = Integer.MIN_VALUE;
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    if (board[i][j] == EMPTY) {
                        board[i][j] = PLAYER_X;
                        int score = minimax(board, depth + 1, false);
                        board[i][j] = EMPTY;
                        bestScore = Math.max(score, bestScore);
                    }
                }
            }
            return bestScore;
        } else {
            int bestScore = Integer.MAX_VALUE;
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    if (board[i][j] == EMPTY) {
                        board[i][j] = PLAYER_O;
                        int score = minimax(board, depth + 1, true);
                        board[i][j] = EMPTY;
                        bestScore = Math.min(score, bestScore);
                    }
                }
            }
            return bestScore;
        }
    }

    private static int checkWin(int[][] board) {
        for (int i = 0; i < 3; i++) {
            if (board[i][0] == board[i][1] && board[i][1] == board[i][2]) {
                return board[i][0] == PLAYER_X ? PLAYER_X_WINS : PLAYER_O_WINS;
            }
        }
        for (int i = 0; i < 3; i++) {
            if (board[0][i] == board[1][i] && board[1][i] == board[2][i]) {
                return board[0][i] == PLAYER_X ? PLAYER_X_WINS : PLAYER_O_WINS;
            }
        }
        if (board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
            return board[0][0] == PLAYER_X ? PLAYER_X_WINS : PLAYER_O_WINS;
        }
        if (board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
            return board[0][2] == PLAYER_X ? PLAYER_X_WINS : PLAYER_O_WINS;
        }
        return DRAW;
    }
}