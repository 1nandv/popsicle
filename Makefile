DEBUG ?= 1

CC=gcc
CFLAGS=-Wall -Wextra

ifeq ($(DEBUG), 1)
	CFLAGS += -DDEBUG -g
endif

all: pop

pop: src/main.c lexer.o io.o
	$(CC) $(CFLAGS) -o $@ $^

lexer.o: src/lexer.c
	$(CC) $(CFLAGS) -c $^ -o $@

io.o: src/io.c
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	rm pop *.o

