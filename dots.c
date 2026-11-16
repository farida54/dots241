#include "dots.h"

int main(void) {
    int r1, c1, r2, c2;

   
    printf("  ");
    for (int c = 0; c < DOT_COLS; c++) {
        printf("%d ", c);
    }
    printf("\n");

    
    for (int r = 0; r < DOT_ROWS; r++) {
        printf("%d ", r);
        for (int c = 0; c < DOT_COLS; c++) {
            printf(". ");
        }
        printf("\n\n");
    }

    printf("Player A's turn. Enter the row and column of the first dot "
           "(e.g., A0 -> 0 0) and second dot: ");
    if (scanf("%d %d %d %d", &r1, &c1, &r2, &c2) != 4) {
        printf("Invalid input.\n");
        return 1;
    }

    printf("You entered: (%d,%d) -> (%d,%d)\n", r1, c1, r2, c2);

    return 0;
}
