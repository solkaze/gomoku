# プログラム名とオブジェクトファイル名
PROGRAM = gomoku

# ソースコード、出力ディレクトリ、ヘッダーディレクトリ
SRC_DIR = src
BIN_DIR = bin
BUILD_DIR = build
INCLUDE_DIR = include

# ソースファイルからオブジェクトファイルのリストを生成
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

CC = g++
CFLAGS = -Wall -O3 -I$(INCLUDE_DIR)

TARGET = $(BIN_DIR)/$(PROGRAM)
# すべてのビルドターゲット
all: $(TARGET)

# 実行ファイルの生成
$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CC) $(OBJS) $(LDFLAGS) -o $@

# オブジェクトファイルの生成ルール (buildフォルダに出力)
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# binディレクトリが存在しない場合は作成
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# buildディレクトリが存在しない場合は作成
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# クリーンアップ
.PHONY: clean
clean:
	$(RM) $(TARGET) $(OBJS)
