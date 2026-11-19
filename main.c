#define _POSIX_C_SOURCE 200809L   /* for sigaction */
#include <stdio.h>
#include <ctype.h>
#include <signal.h>
#include "board.h"

/* set by the Ctrl+C handler; the game checks it after every read */
static volatile sig_atomic_t quit_requested = 0;

static void on_ctrl_c(int sig) {
    (void)sig;
    quit_requested = 1;          /* only set a flag: printf is not safe here */
}

/* Reads one line and keeps only its digits, ignoring all whitespace, so
   "0 0 0 1", "0001", "00 01" and "0 0 01" are the same move. Every
   coordinate is one digit (rows 0-4, columns 0-5).
   Returns 1 with move[] filled, 0 if the line is not exactly 4 digits,
   -1 if the input ended or Ctrl+C was pressed. */
static int read_move(int move[4]) {
    int ch, count = 0, bad = 0, seen = 0;

    while ((ch = getchar()) != '\n') {
        if (ch == EOF) {
            if (quit_requested || !seen) return -1;
            break;                       /* last line had no newline: use it */
        }
        seen = 1;
        if (isspace(ch)) continue;       /* spaces, tabs, '\r' from Windows files */
        if (!isdigit(ch) || count == 4) {
            bad = 1;                     /* a letter, a '-', or a 5th digit */
            continue;                    /* keep reading to the end of the line */
        }
        move[count++] = ch - '0';
    }
    if (quit_requested) return -1;
    return (!bad && count == 4) ? 1 : 0;
}

/* Plays one full game. Returns 1 when all boxes were claimed,
   0 if the input ended or Ctrl+C was pressed. */
static int play_game(void) {
    int r1, c1, r2, c2;
    int move[4];
    int h[DOT_ROWS][DOT_COLS - 1] = {0};
    int v[DOT_ROWS - 1][DOT_COLS] = {0};
    char box[BOX_ROWS][BOX_COLS];
    char player = 'A';
    int lines_drawn = 0;
    int scoreA = 0, scoreB = 0;

    for (int r = 0; r < BOX_ROWS; r++) {
        for (int c = 0; c < BOX_COLS; c++) {
            box[r][c] = ' ';
        }
    }

    while (lines_drawn < TOTAL_LINES) {
        print_board(h, v, box);

        printf("Player %c's turn. Enter the row and column of the first dot "
               "(e.g., A0 -> 0 0) and second dot: ", player);
        fflush(stdout);                  /* show the prompt before waiting */

        int status = read_move(move);
        if (status < 0) return 0;
        if (status == 0) {
            printf("Invalid input, enter 4 digits (e.g. 0 0 0 1 or 0001).\n");
            continue;                    /* same player again */
        }
        r1 = move[0]; c1 = move[1]; r2 = move[2]; c2 = move[3];

        if (!draw_line(h, v, r1, c1, r2, c2)) {
            printf("Invalid move, try again.\n");
            continue;
        }
        lines_drawn++;

        int completed = check_boxes(h, v, box, r1, c1, r2, c2, player);
        if (completed > 0) {
            if (player == 'A') scoreA += completed;
            else               scoreB += completed;
            printf("Player %c completed %d box(es) and plays again.\n",
                   player, completed);
        } else {
            player = (player == 'A') ? 'B' : 'A';  /* switch only if no box */
        }

        /* score banner after every valid move, like the sample output */
        printf("**********************************************************************\n");
        printf("Player A score: %d\n", scoreA);
        printf("Player B score: %d\n", scoreB);
        printf("**********************************************************************\n");
    }

    print_board(h, v, box);
    printf("Final score  A: %d  B: %d\n", scoreA, scoreB);
    if (scoreA > scoreB)      printf("Player A wins!\n");
    else if (scoreB > scoreA) printf("Player B wins!\n");
    else                      printf("It's a tie!\n");
    return 1;
}

/* Returns 1 if the player answers y or yes, 0 for anything else,
   end of input or Ctrl+C. */
static int ask_play_again(void) {
    int ch, answer = 0;

    printf("Play again? (y/n): ");
    fflush(stdout);
    while ((ch = getchar()) != '\n' && ch != EOF) {
        if (answer == 0 && !isspace(ch)) answer = ch;   /* first real letter */
    }
    if (quit_requested) return 0;
    return answer == 'y' || answer == 'Y';
}

int main(void) {
    struct sigaction sa = {0};
    sa.sa_handler = on_ctrl_c;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;                     /* no SA_RESTART: Ctrl+C stops the read */
    sigaction(SIGINT, &sa, NULL);

    printf("Dots and Boxes - press Ctrl+C at any time to quit.\n");

    int finished;
    while ((finished = play_game()) && ask_play_again()) {
        printf("\nNew game!\n");
    }

    if (quit_requested) {
        printf("\nGame stopped with Ctrl+C. Goodbye!\n");
        return 0;
    }
    if (!finished) {
        printf("\nInput ended.\n");
        return 1;                        /* stopped in the middle of a game */
    }
    printf("Thanks for playing!\n");
    return 0;
}
