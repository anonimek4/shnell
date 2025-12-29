CC = gcc
CFLAGS = -Iinclude -Wall -Wextra -g
SRC = src/main.c src/utils.c
OBJ = $(SRC:.c=.o)
TARGET = bin/shnell

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
