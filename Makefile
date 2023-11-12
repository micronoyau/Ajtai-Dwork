CC=gcc
CFLAGS=-g -Wall

SRC_DIR=src
SRC_FILES=$(wildcard $(SRC_DIR)/*.c)

TARGET_DIR=target
OBJ_DIR=$(TARGET_DIR)/obj
OBJ_FILES=$(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))
LIB_DIR=$(TARGET_DIR)/lib
LIB_NAME=crypto
LIB_FILE=$(LIB_DIR)/lib$(LIB_NAME).a

TEST_DIR=tests
TEST_BIN_DIR=$(TEST_DIR)/bin
TEST_SRC_FILES=$(wildcard $(TEST_DIR)/*.c)
TEST_BIN_FILES=$(patsubst $(TEST_DIR)/%.c, $(TEST_BIN_DIR)/%.test, $(TEST_SRC_FILES))

all: test


# Build

build: $(LIB_FILE)
	echo "Build complete !"

$(LIB_FILE): $(OBJ_FILES) | $(LIB_DIR)
	ar crsv $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $< $(CFLAGS) -c -o $@

$(OBJ_DIR): $(TARGET_DIR)
	mkdir -p $@

$(LIB_DIR): $(TARGET_DIR)
	mkdir $@

$(TARGET_DIR):
	mkdir $@


# Tests

test: $(TEST_BIN_FILES)
	echo -e "[*] TESTS...\n"
	for t in $^; do \
		./$$t && echo -e "[+] Test $$t done\n"; \
	done
	echo "[+] All tests complete !"

$(TEST_BIN_DIR)/%.test: $(TEST_DIR)/%.c build | $(TEST_BIN_DIR)
	$(CC) $< $(CFLAGS) -I $(SRC_DIR) -lm -L $(LIB_DIR) -l $(LIB_NAME) -o $@

$(TEST_BIN_DIR):
	mkdir $@

# Clean

clean: $(TARGET_DIR)
	rm -r $(TARGET_DIR)
	rm -r $(TEST_BIN_DIR)
