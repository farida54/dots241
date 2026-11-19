#include <stdio.h>
#include <string.h>
#include "board.h"

int main(void) {
    int r1, c1, r2, c2;
    char line[128];
    char extra;
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
        /* read one whole line so leftover text never leaks into the next turn */
        if (fgets(line, sizeof line, stdin) == NULL) {
            printf("\nInput ended.\n");
            return 1;
        }
        if (strchr(line, '\n') == NULL) {          /* line too long: drop the rest */
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) { }
        }
        if (sscanf(line, "%d %d %d %d %c", &r1, &c1, &r2, &c2, &extra) != 4) {
            printf("Invalid input, enter exactly 4 numbers (e.g. 0 0 0 1).\n");
            continue;                              /* same player again */
        }

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

    
    return 0;
}
