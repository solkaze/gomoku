# Makefile

# プログラム名とオブジェクトファイル名
PROGRAM = gomoku

# ソースコードディレクトリと出力ディレクトリ
SRC_DIR = src
BIN_DIR = bin

OBJS = $(BIN_DIR)/gomoku.o $(BIN_DIR)/gomoku_ai.o $(BIN_DIR)/.libgomoku.o

CC = g++
CFLAGS = -Wall -O3

# すべてのビルドターゲット
all: $(BIN_DIR)/$(PROGRAM)

# 実行ファイルの生成
$(BIN_DIR)/$(PROGRAM): $(OBJS) | $(BIN_DIR)
	$(CC) $(OBJS) $(LDFLAGS) -o $@

# オブジェクトファイルの生成ルール
$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BIN_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# binディレクトリが存在しない場合は作成
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# クリーンアップ
.PHONY: clean
clean:
	$(RM) $(BIN_DIR)/$(PROGRAM) $(OBJS)
