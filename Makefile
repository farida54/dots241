CC = gcc
CFLAGS = -Wall -Wextra -g -O0
OBJS = main.o board.o
PREFIX = /usr/local

VALGRIND_FLAGS = --leak-check=full --show-leak-kinds=all --track-origins=yes --error-exitcode=1

dots: $(OBJS)
	$(CC) $(CFLAGS) -o dots $(OBJS)

main.o: main.c board.h dots.h
	$(CC) $(CFLAGS) -c main.c

board.o: board.c board.h dots.h
	$(CC) $(CFLAGS) -c board.c

# Play a full scripted game and an input that ends early under Valgrind.
# Logs are saved in debugging/.
valgrind: dots
	valgrind $(VALGRIND_FLAGS) ./dots < tests/full_game.txt > /dev/null 2> debugging/valgrind_full_game.txt
	valgrind $(VALGRIND_FLAGS) ./dots < tests/early_eof.txt > /dev/null 2> debugging/valgrind_early_eof.txt || true  # game exits 1 on early EOF by design
	@grep -h "ERROR SUMMARY\|All heap blocks" debugging/valgrind_*.txt

# Scripted GDB session (breakpoints, backtrace, inspecting the board).
gdb-session: dots
	gdb -q -batch -x debugging/session.gdb ./dots 2>&1 | tee debugging/gdb_session.txt

# Used by alpine/setup.sh inside the VM.
install: dots
	install -d $(PREFIX)/bin
	install -m 755 dots $(PREFIX)/bin/dots
	install -m 755 alpine/dots-launch $(PREFIX)/bin/dots-launch

clean:
	rm -f dots $(OBJS)

.PHONY: clean valgrind gdb-session install
