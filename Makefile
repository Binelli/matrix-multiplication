CC=gcc
CFLAGS=-O3
LIBS=
SRC_DIR=./src
OBJ_DIR=obj
BUILD_DIR=build

TARGETS=$(BUILD_DIR)/matrix_1

_OBJS = matrix_1.o
OBJS = $(patsubst %,$(OBJ_DIR)/%,$(_OBJS))

all: $(TARGETS)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) -c -o $@ $< $(CFLAGS)

$(BUILD_DIR)/matrix_1: $(OBJS) | $(BUILD_DIR)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

$(BUILD_DIR):
	mkdir -p $@

.PHONY: clean

clean:
	rm -f $(OBJ_DIR)/* $(BUILD_DIR)/*
