#include <stdio.h>

#define DOT_ROWS 5
#define DOT_COLS 6
#define BOX_ROWS (DOT_ROWS - 1)
#define BOX_COLS (DOT_COLS - 1)

/* 5 rows x 5 horizontal lines + 4 rows x 6 vertical lines */
#define TOTAL_LINES ((DOT_ROWS * (DOT_COLS - 1)) + ((DOT_ROWS - 1) * DOT_COLS))
