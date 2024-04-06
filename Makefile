CC=gcc
# CFLAGS=-g

all: pop

pop: src/main.c lexer.o  
	$(CC) -o $@ $^

lexer.o: src/lexer.c
	$(CC) -c $^ -o $@

clean:
	rm pop *.o

