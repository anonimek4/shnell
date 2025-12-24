CC = gcc
CFLAGS = -Iinclude -Wall -Wextra
SRC = src/main.c
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
