SRC := src
OBJ := obj
BIN := bin
EXECUTABLE:= shell
SRCS := $(wildcard $(SRC)/*.c)
OBJS := $(patsubst $(SRC)/%.c,$(OBJ)/%.o,$(SRCS))
INCS := -Iinclude/
DIRS := $(OBJ)/ $(BIN)/
EXEC := $(BIN)/$(EXECUTABLE)
CC := gcc
CFLAGS := -g -Wall -std=c99 $(INCS)
LDFLAGS :=

all: $(EXEC)

$(EXEC): $(OBJS) | $(BIN)
	$(CC) $(CFLAGS) $(OBJS) -o $(EXEC)

$(OBJ)/%.o: $(SRC)/%.c | $(OBJ)
	$(CC) $(CFLAGS) -c $< -o $@

run: $(EXEC)
	$(EXEC)

clean:
	rm -f $(OBJ)/*.o $(EXEC)
	rmdir $(OBJ) $(BIN) 2>/dev/null || true

$(OBJ):
	mkdir -p $(OBJ)

$(BIN):
	mkdir -p $(BIN)

.PHONY: run clean all