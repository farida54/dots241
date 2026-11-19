#ifndef BOARD_H
#define BOARD_H

#include "dots.h"

/* draws the grid with lines and box owners */
void print_board(int h[DOT_ROWS][DOT_COLS - 1], int v[DOT_ROWS - 1][DOT_COLS],
                 char box[BOX_ROWS][BOX_COLS]);

/* returns 1 if the line was drawn, 0 if the move is not allowed */
int draw_line(int h[DOT_ROWS][DOT_COLS - 1], int v[DOT_ROWS - 1][DOT_COLS],
              int r1, int c1, int r2, int c2);

/* returns 1 if box (r,c) has all 4 sides drawn */
int box_complete(int h[DOT_ROWS][DOT_COLS - 1], int v[DOT_ROWS - 1][DOT_COLS],
                 int r, int c);

/* gives box (r,c) to player if it was just completed; returns 1 or 0 */
int try_claim(int h[DOT_ROWS][DOT_COLS - 1], int v[DOT_ROWS - 1][DOT_COLS],
              char box[BOX_ROWS][BOX_COLS], int r, int c, char player);

/* checks the boxes next to the new line; returns how many were completed */
int check_boxes(int h[DOT_ROWS][DOT_COLS - 1], int v[DOT_ROWS - 1][DOT_COLS],
                char box[BOX_ROWS][BOX_COLS],
                int r1, int c1, int r2, int c2, char player);

#endif
