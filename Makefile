CC = gcc
CFLAGS = -Wall -Wextra -g -O0
OBJS = main.o board.o

dots: $(OBJS)
	$(CC) $(CFLAGS) -o dots $(OBJS)

main.o: main.c board.h dots.h
	$(CC) $(CFLAGS) -c main.c

board.o: board.c board.h dots.h
	$(CC) $(CFLAGS) -c board.c

clean:
	rm -f dots $(OBJS)

.PHONY: clean
