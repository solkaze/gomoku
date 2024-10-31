//------------------------------------------------------------------------
// 五目ならべAI
//  ここを作る．
//  下記のサンプルは，直前に自分が置いた石の8近傍のどこかにランダムに石を置くだけ
//
//  引数説明
//    board[BOARD_SIZE][BORARD_SIZE]：五目並べの盤
//    com ： コンピュータが白石(STONE_WHITE)か黒石(STONE_BLACK)かを表す．
//    pos_x, pos_y：石を置く場所のx座標，y座標 両方出力用
//------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <vector>
#include <cmath>
#include "gomoku.hpp"

const int max_depth = 3;

int evaluate_board(std::vector<std::vector<int>>& board, int x, int y, int com)
{
    int score = 0;
    int directions[4][2] = { {1, 0}, {0, 1}, {1, 1}, {1, -1} };
    int three_count = 0;

    for (int d = 0; d < 4; d++)
    {
        int count = 1;
        int open_ends = 0;
        for (int i = 1; i <= 4; i++)
        {
            int nx = x + i * directions[d][0];
            int ny = y + i * directions[d][1];
            if (nx < 0 || nx >= BOARD_SIZE || ny < 0 || ny >= BOARD_SIZE)
                break;
            if (board[ny][nx] == com)
                count++;
            else
            {
                if (board[ny][nx] == STONE_SPACE)
                    open_ends++;
                break;
            }
        }

        for (int i = 1; i <= 4; i++)
        {
            int nx = x - i * directions[d][0];
            int ny = y - i * directions[d][1];
            if (nx < 0 || nx >= BOARD_SIZE || ny < 0 || ny >= BOARD_SIZE)
                break;
            if (board[ny][nx] == com)
                count++;
            else
            {
                if (board[ny][nx] == STONE_SPACE)
                    open_ends++;
                break;
            }
        }

        // 相手が4連を形成している場合に高スコアを付与
        if (com != (STONE_BLACK) && com != (STONE_WHITE)) {
            // 無効なcom値のチェック（念のため）
            continue;
        }

        if (com != (com == STONE_BLACK ? STONE_BLACK : STONE_WHITE)) { // 相手の場合
            if (count == 4)
            {
                score += 100000; // 相手の4連に対する非常に高いスコア
            }
            else if (count == 5)
            {
                score += 1000000; // 相手が5連の場合、即座に防ぐ必要がある
            }
            // 他のスコアリングは変更しない
        }
        else
        {
            if (count == 5)
            {
                score += 10000; // 5つ揃う評価値
            }
            else if (count == 4)
            {
                score += 1000; // 4つ揃う評価値
            }
            else if (count == 3)
            {
                if (open_ends == 2)
                {
                    score += 150; // 両側が開いている3つ揃い
                }
                else if (open_ends == 1)    
                {
                    score += 50; // 片側がブロックされている3つ揃い
                }
            }
            else if (count == 2)
            {
                if (open_ends == 2)
                {
                    score += 15; // 両側が開いている2つ揃い
                }
                else if (open_ends == 1)
                {
                    score += 5; // 片側がブロックされている2つ揃い
                }
            }
        }

        if (com == STONE_BLACK) // 黒石（先行）の場合のみ三三と長連禁止を適用
        {
            if (count == 3) // 三三禁止のチェック
            {
                three_count++;
            }

            if (count > 5) // 長連禁止のチェック
            {
                score = 0; // 長連は禁止なので、評価値を無効にする
            }
        }
    }

    // 黒石（先行）が三三を作っている場合のペナルティ
    if (com == STONE_BLACK && three_count >= 2)
    {
        score = 0; // 大きなペナルティを与える
    }

    return score;
}

int alpha_beta(std::vector<std::vector<int>>& board, int depth, int alpha, int beta, bool isMaximizing) {
    if (depth == 0 || /*勝利条件*/) {
        return evalute_board();
    }

	int ai_x = BOARD_SIZE / 2;
	int ai_y = BOARD_SIZE / 2;
	int move_length = 1;
	int direction_p = 0;

	int board_max = BOARD_SIZE * BOARD_SIZE;

	// 確認済みカウント
	int count = 0;

    if (isMaximizing) {
        int maxEval = std::numeric_limits<int>::min();
		// らせん型探索に変更予定
		while (count < board_max) {
			for (int i = 0; i < 2; ++i) {
				int dx = std::cos(direction_p * M_PI / 2);
				int dy = std::sin(direction_p * M_PI / 2);

				for (int j = 0; j < move_length; ++j) {
					if (board[ai_y][ai_x] == 0) {
						board[ai_y][ai_x] = 1;
						int eval = alpha_beta(board, depth - 1, alpha, beta, false);
						board[ai_y][ai_x] = 0;
						maxEval = std::max(maxEval, eval);
						alpha = std::max(alpha, eval);
						if (beta <= alpha) break;
					}
					++count;
					ai_x += dx;
					ai_y += dy;
				}
				direction_p = (direction_p + 1) % 4;
			}
			++move_length;
		}
        return maxEval;

    } else {
        int minEval = std::numeric_limits<int>::max();
        while (count < board_max) {
			for (int i = 0; i < 2; ++i) {
				int dx = std::cos(direction_p * M_PI / 2);
				int dy = std::sin(direction_p * M_PI / 2);

				for (int j = 0; j < move_length; ++j) {
					if (board[ai_y][ai_x] == 0) {
						board[ai_y][ai_x] = -1;
						int eval = alpha_beta(board, depth - 1, alpha, beta, true);
						board[ai_y][ai_x] = 0;
						minEval = std::min(minEval, eval);
						beta = std::min(beta, eval);
						if (beta <= alpha) break;
					}
					++count;
					ai_x += dx;
					ai_y += dy;
				}
				direction_p = (direction_p + 1) % 4;
			}
			++move_length;
		}
        return minEval;
    }
}

void find_best_move(std::vector<std::vector<int>>& board) {
    int board_size = board.size();
	int ai_x = board_size / 2;
	int ai_y = board_size / 2;
	int move_length = 1;
	int direction_p = 0;

	int board_max = board_size * board_size;

	// 確認済みカウント
	int count = 0;
    int bestVal = std::numeric_limits<int>::min();
    std::pair<int, int> bestMove = {-1, -1};

	while (count < board_max) {
		for (int i = 0; i < 2; ++i) {
			int dx = std::cos(direction_p * M_PI / 2);
			int dy = std::sin(direction_p * M_PI / 2);

			for (int j = 0; j < move_length; ++j) {
				if (board[i][j] == 0) {
					board[i][j] = 1;
					int moveVal = alpha_beta(board, max_depth, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), false);
					board[i][j] = 0;

					if (moveVal > bestVal) {
						bestMove = {i, j};
						bestVal = moveVal;
					}
				}
				++count;
				ai_x += dx;
				ai_y += dy;
			}
			direction_p = (direction_p + 1) % 4;
		}
		++move_length;
	}
    board[bestMove.first][bestMove.second] = -1;
}

int calcPutPos(int board[][BOARD_SIZE], int com, int *pos_x, int *pos_y)
{

    return 0;
}
