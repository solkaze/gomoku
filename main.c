#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

#define MAX_DEPTH 4

#define BOARD_SIZE 15
#define EMPTY 0
#define PLAYER 1
#define AI 2

typedef struct {
    int board[BOARD_SIZE][BOARD_SIZE];
} Gomoku;

// 盤面を初期化する関数
void initBoard(Gomoku *game) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            game->board[i][j] = EMPTY;
        }
    }
}

// 盤面を表示する関数
void printBoard(Gomoku *game) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            printf("%d ", game->board[i][j]);
        }
        printf("\n");
    }
}

// 有効な手を取得する関数
int getValidMoves(Gomoku *game, int moves[][2]) {
    int count = 0;
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (game->board[i][j] == EMPTY) {
                moves[count][0] = i;
                moves[count][1] = j;
                count++;
            }
        }
    }
    return count;
}

// 石を置く関数
void makeMove(Gomoku *game, int y, int x, int player) {
    game->board[y][x] = player;
}

// 石を戻す関数
void undoMove(Gomoku *game, int y, int x) {
    game->board[y][x] = EMPTY;
}

// 特定の方向に同じ石が5つ並んでいるか確認する関数
bool checkDirection(Gomoku *game, int startX, int startY, int dy, int dx, int player) {
    for (int i = 0; i < 5; i++) {
        int x = startX + i * dx;
        int y = startY + i * dy;
        
        // 盤面外に出るか、石がプレイヤーのものではない場合は失敗
        if (x < 0 || x >= BOARD_SIZE || y < 0 || y >= BOARD_SIZE || game->board[y][x] != player) {
            return false;
        }
    }
    return true;
}

// 盤面全体をチェックして勝利者がいるかどうかを判定する関数
int checkWinner(Gomoku *game) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (game->board[i][j] == EMPTY) {
                continue;
            }

            // 現在のマスにある石がPLAYERかAIか
            int player = game->board[i][j];

            // 横方向に5つ並んでいるか確認
            if (checkDirection(game, i, j, 0, 1, player)) {
                return player;
            }

            // 縦方向に5つ並んでいるか確認
            if (checkDirection(game, i, j, 1, 0, player)) {
                return player;
            }

            // 右斜め方向に5つ並んでいるか確認
            if (checkDirection(game, i, j, 1, 1, player)) {
                return player;
            }

            // 左斜め方向に5つ並んでいるか確認
            if (checkDirection(game, i, j, 1, -1, player)) {
                return player;
            }
        }
    }

    // 勝者がいない場合は0を返す
    return 0;
}


// 評価関数（仮実装）
int evaluate(Gomoku *game) {
    // 評価関数を適切に実装する必要あり
    return 0;
}

// アルファ・ベータカットによるミニマックス法
int alphaBeta(Gomoku *game, int depth, int alpha, int beta, int isMaximizing) {
    if (depth == 0 || checkWinner(game)) {
        return evaluate(game);
    }

    int moves[BOARD_SIZE * BOARD_SIZE][2]; // y, xの２種;
    int moveCount = getValidMoves(game, moves);

    if (isMaximizing) {
        int maxEval = INT_MIN;
        for (int i = 0; i < moveCount; i++) {
            makeMove(game, moves[i][0], moves[i][1], AI);
            int eval = alphaBeta(game, depth - 1, alpha, beta, 0);
            undoMove(game, moves[i][0], moves[i][1]);

            maxEval = max(maxEval, eval);
            alpha = max(alpha, maxEval);

            if (beta <= alpha) {
                break; // ベータカットオフ
            }
        }
        return maxEval;
    } else {
        int minEval = INT_MAX;
        for (int i = 0; i < moveCount; i++) {
            makeMove(game, moves[i][0], moves[i][1], PLAYER);
            int eval = alphaBeta(game, depth - 1, alpha, beta, 1);
            undoMove(game, moves[i][0], moves[i][1]);
            if (eval < minEval) {
                minEval = eval;
            }
            if (eval < beta) {
                beta = eval;
            }
            if (beta <= alpha) {
                break; // アルファ剪定
            }

            minEval = min(minEval, eval);
            beta = min(beta, minEval);

            if (beta <= alpha) {
                break; // アルファカットオフ
            }
        }
        return minEval;
    }
}

// 最善の手を見つける関数
void findBestMove(Gomoku *game, int depth, int *bestY, int *bestX) {
    int bestValue = INT_MIN;
    int moves[BOARD_SIZE * BOARD_SIZE][2];
    int moveCount = getValidMoves(game, moves);

    for (int i = 0; i < moveCount; i++) {
        makeMove(game, moves[i][0], moves[i][1], AI);
        int moveValue = alphaBeta(game, depth - 1, INT_MIN, INT_MAX, 0);
        undoMove(game, moves[i][0], moves[i][1]);

        if (moveValue > bestValue) {
            bestValue = moveValue;
            *bestY = moves[i][0];
            *bestX = moves[i][1];
        }
    }
}

int main() {
    Gomoku game;
    initBoard(&game);
    printBoard(&game);

    while (!checkWinner(&game)) {
        int x, y;

        // プレイヤーの手を入力
        printf("Enter your move (row and column): ");
        scanf("%d %d", &y, &x);
        makeMove(&game, y, x, PLAYER);
        printBoard(&game);

        if (checkWinner(&game)) {
            printf("Player wins!\n");
            break;
        }

        // AIの手を決定
        int bestX, bestY;
        findBestMove(&game, MAX_DEPTH, &bestY, &bestX); // 探索深さは4に設定
        makeMove(&game, bestX, bestY, AI);
        printf("AI plays: %d, %d\n", bestY, bestX);
        printBoard(&game);

        if (checkWinner(&game)) {
            printf("AI wins!\n");
            break;
        }
    }

    return 0;
}
