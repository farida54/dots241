#include "board.h"
#include <stdlib.h>

/* h[r][c] = 1 IS line between dot (r,c) and dot (r,c+1) */
/* v[r][c] = 1 IS line between dot (r,c) and dot (r+1,c) */
/* box[r][c]    ' ' if free, 'A' or 'B' if claimed       */



void print_board(int h[DOT_ROWS][DOT_COLS - 1], int v[DOT_ROWS - 1][DOT_COLS],
                 char box[BOX_ROWS][BOX_COLS]) {
    printf("  ");
    for (int c = 0; c < DOT_COLS; c++) {
        printf("%d ", c);
    }
    printf("\n");

    
    for (int r = 0; r < DOT_ROWS; r++) {
        printf("%d ", r);
        for (int c = 0; c < DOT_COLS; c++) {
            printf(".");
            if (c < DOT_COLS - 1) {
                printf("%c", h[r][c] ? '-' : ' ');
            }
        }
        printf("\n");

        // vertical lines and box owners between this row and the next 



        printf("  ");
        if (r < DOT_ROWS - 1) {
            for (int c = 0; c < DOT_COLS; c++) {
                printf("%c", v[r][c] ? '|' : ' ');
                if (c < DOT_COLS - 1) {
                    printf("%c", box[r][c]);
                }
            }
        }
        printf("\n");
    }
}

/* returns 1 if the line was drawn, 0 if the move is not allowed */
int draw_line(int h[DOT_ROWS][DOT_COLS - 1], int v[DOT_ROWS - 1][DOT_COLS],
              int r1, int c1, int r2, int c2) {
    /* both dots must be on the board */
    if (r1 < 0 || r1 >= DOT_ROWS || r2 < 0 || r2 >= DOT_ROWS ||
        c1 < 0 || c1 >= DOT_COLS || c2 < 0 || c2 >= DOT_COLS) {
        return 0;
    }

    if (r1 == r2 && abs(c1 - c2) == 1) {          // horizontal 
        int c = (c1 < c2) ? c1 : c2;
        if (h[r1][c]) return 0;                    // already drawn
        h[r1][c] = 1;
        return 1;
    }

    if (c1 == c2 && abs(r1 - r2) == 1) {          /* vertical */
        int r = (r1 < r2) ? r1 : r2;
        if (v[r][c1]) return 0;                    /* already drawn */
        v[r][c1] = 1;
        return 1;
    }

    return 0;                                      /* dots not next to each other */
}

/* returns 1 if box (r,c) has all 4 sides drawn */
int box_complete(int h[DOT_ROWS][DOT_COLS - 1], int v[DOT_ROWS - 1][DOT_COLS],
                 int r, int c) {
    return h[r][c] && h[r + 1][c] && v[r][c] && v[r][c + 1];
}

/* If box (r,c) is on the board, still free and now complete, give it to
   player. Returns 1 if the box was claimed, 0 otherwise. */



int try_claim(int h[DOT_ROWS][DOT_COLS - 1], int v[DOT_ROWS - 1][DOT_COLS],
              char box[BOX_ROWS][BOX_COLS], int r, int c, char player) {
    if (r < 0 || r >= BOX_ROWS || c < 0 || c >= BOX_COLS) return 0;
    if (box[r][c] != ' ') return 0;
    if (!box_complete(h, v, r, c)) return 0;
    box[r][c] = player;
    return 1;
}

/* Checks the 1 or 2 boxes touching the line that was just drawn.
   Returns how many boxes were completed (0, 1 or 2). */



int check_boxes(int h[DOT_ROWS][DOT_COLS - 1], int v[DOT_ROWS - 1][DOT_COLS],
                char box[BOX_ROWS][BOX_COLS],
                int r1, int c1, int r2, int c2, char player) {
    int r = (r1 < r2) ? r1 : r2;
    int c = (c1 < c2) ? c1 : c2;
    int count = 0;

    if (r1 == r2) {                                /* horizontal line */
        count += try_claim(h, v, box, r - 1, c, player);   /* box above */
        count += try_claim(h, v, box, r, c, player);       /* box below */
    } else {                                       /* vertical line */
        count += try_claim(h, v, box, r, c - 1, player);   /* box left  */
        count += try_claim(h, v, box, r, c, player);       /* box right */
    }
    return count;
}
