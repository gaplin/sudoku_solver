#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sudoku.h"

int main(int argc, char* argv[]) {
    reseed_rand(time(NULL));
    int puzzle[SUDOKU_NN][SUDOKU_NN];

    for(int i = 0; i < SUDOKU_NN; ++i) {
        for(int j = 0; j < SUDOKU_NN; ++j) {
            if(scanf("%d", &puzzle[i][j]) != 1 || puzzle[i][j] < 0 || puzzle[i][j] > SUDOKU_NN) {
                printf("Invalid input value. Only digits are acceptable.\n");
                return -1;
            }
        }
    }

    int (*solution)[SUDOKU_NN] = (int(*)[SUDOKU_NN]) get_random_solution(puzzle);
    if(solution == NULL) {
        printf("No solution found.\n");
        return 0;
    }

    for(int i = 0; i < SUDOKU_NN; ++i) {
        for(int j = 0; j < SUDOKU_NN; ++j) {
            printf("%d ", solution[i][j]);
        }
        printf("\n");
    }
    free(solution);
}